/**
 * subproc.c
 *
 * This file contains the internal data and function definitions for the 
 * subproc type.
 *
 * The subproc type executes shell commands as a sub/child process.
 *
 * Author: Richard Gale
 * Version: 1.0.1
 */

#include "subproc.h"

/**
 * This is the internal data contained within the subproc type.
 */
struct subproc_data {
    int fds[2]; /* File descriptors. */
    pid_t pid;  /* Process Id. */
};

/**
 * This function initialises the subproc provided to it.
 */
void subproc_init(subproc* sp)
{
    /* Allocate memory to the subroc. */
    *sp = (subproc) malloc(sizeof(struct subproc_data));
}

/**
 * This function destroys the subproc provided to it.
 */
void subproc_free(subproc* sp)
{
    /* De-allocate memory from the subroc. */
    free(*sp);
}

/**
 * This function duplicates the "old" file descriptor provided to it. If there
 * is an error it is printed on stderr and the program exits.
 *
 * dup2() is used to specify the file descriptor being used for a pipe(),
 * file stream, etc...
 */
void duperr(int fdold, int fdnew)
{
    /* Attempting to duplicate the file descriptor. */
    if (dup2(fdold, fdnew) == -1) 
    {    
        /* There was an error duplicating the file descriptor so print it and
         * exit the program. */
        fprintf(stderr, 
                "[ %s ] dup2 failed on fileno() %s\n", 
                timestamp(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * This function creates a file name from a directory path, a shell command,
 * and a file extension.
 */
void mkfname(char** sp, char* dir, char* cmd, char* ext)
{
    /* Remove unwanted characters from the copy. */
    sdelchar(&cmd, '/');
    sdelchar(&cmd, '.');

    /* Create the file name. */
    strfmt(sp, "%s%s%s", dir, cmd, ext);
}

/**
 * This function executes the command provided to it as a sub-process.
 */
void subproc_exec( subproc* sp, char* cmd, char* fdir )
{
    FILE* fout;         /* The file stream for stdout. */
    FILE* ferr;         /* The file stream for stderr. */
    char* tstamp;       /* A timestamp. */
    char* fname_out;    /* The file name for the stdout file stream. */
    char* fname_err;    /* The file name for the stderr file stream. */
    
    /* The file name extensions. */
    char* fext_out = "_out.txt";
    char* fext_err = "_err.txt";

    /* Create a pipe to use for the child process. */
    if (pipe((*sp)->fds) == -1)
    {
        /* There was an error creating the pipe so print it and exit the
         * program. */
        fprintf(stderr,
                "[ %s ] ERROR: In subproc_exec(): pipe() - %s\n", 
                (tstamp = timestamp()), strerror(errno));
        free(tstamp);
        exit(EXIT_FAILURE);
    }

    /* Create the child process. */
    if (((*sp)->pid = fork()) == -1)
    {
        /* There was an error creating the child process so print it and
         * exit the program. */
        fprintf(stderr,
                "ERROR: In subproc_exec() fork() - %s\n", 
                (tstamp = timestamp()), strerror(errno));
        free(tstamp);
        exit(EXIT_FAILURE);
    }
    else if ((*sp)->pid == 0)   /* The child process. */
    {
        /* Print status message. */
        fprintf(stdout,
                "[ %s ] Creating sub-process...\n", 
                (tstamp = timestamp()));
        free(tstamp);

        /* Create the file names for the output information. */
        mkfname(&fname_out, fdir, cmd, fext_out);
        mkfname(&fname_err, fdir, cmd, fext_err);

        /* Set the read descriptor of the child process to be
         * stdin's descriptor. */
        duperr(((*sp)->fds )[0], STDIN_FILENO);

        /* Open the file streams. */
        fout = openfs(fname_out, "w");
        ferr = openfs(fname_err, "w");

        /* Setting the descriptor of the output file's stream to be
         * stdout's descriptor. */
        duperr(fileno(fout), STDOUT_FILENO);

        /* Set the descriptor of the error file's stream to be
         * stderr's descriptor. */
        duperr(fileno(ferr), STDERR_FILENO);

        /* Now that the descriptors have been set, close the file streams
         * and free the memory allocated to the file names. */
        closefs(fout);
        closefs(ferr);
        free(fname_out);
        free(fname_err);

        /* Print a status message. */
        fprintf(stdout, 
                "[ %s ] Sub-process created... Executing command...\n",
                (tstamp = timestamp()));
        free(tstamp);

        /* Execute the command as the child process. */
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        
        /* There was an error executing the command so we are print it. */
        fprintf(stderr, 
                "[ %s ] execl failed with error - %s", 
                (tstamp = timestamp()), strerror( errno ));
        free(tstamp);
    }
}

/**
 * This function requests for the provided sub-process to be terminated, waits
 * for it to exit, and reports its exit-status to stdout or stderr if there
 * was an error.
 */
void subproc_term( subproc* sp )
{
    int status;     /* The exit status of the process. */
    char* tstamp;   /* A time stamp. */

    /* Print a status message. */
    fprintf(stdout, 
            "[ %s ] Terminating sub-process...\n", 
            (tstamp = timestamp()));
    free(tstamp);

    /* Terminate the process. */
    kill( ( *sp )->pid, SIGTERM);

    /* Wait for the process to exit and then print its exit status. */
    do
    {
        if (((*sp)->pid = waitpid((*sp)->pid, &status, WNOHANG)) == -1)
        {
            /* There was an error waiting for the process to exit so print
             * the error. */
            fprintf(stderr, 
                    "[ %s ] ERROR: in subproc_term(): wait() error!\n",
                    (tstamp = timestamp()));
            free(tstamp);
        }
        else if ((*sp)->pid == 0)
        {
            /* The process is still running so print a status message and wait
             * for one second. */
            fprintf(stdout,
                    "[ %s ] Waiting for process to terminate...\n",
                    (tstamp = timestamp()));
            free(tstamp);
            sleep (1);
        }
        else
        {
            /* There is no longer a process with the pid so look at what
             * happened to it. */
            if (WIFEXITED(status))
            {
                /* The process exited normally so print its exit status. */
                fprintf(stdout,
                        "[ %s ] The process exited normally with exit"
                        " status %d.\n", 
                        (tstamp = timestamp()), WEXITSTATUS(status));
                free(tstamp);
            }
            else if (WIFSIGNALED(status))
            {
                /* The process exited because of an uncaught signal. */
                fprintf(stdout, 
                        "[ %s ] The process did not exit normally\n",
                        (tstamp = timestamp()));
                free(tstamp);
            }
            else
            {
                /* The process did not exit. */
                fprintf(stdout, 
                        "[ %s ] The child process did not exit\n",
                        (tstamp = timestamp()));
                free(tstamp);
            }
        }
    } while ((*sp)->pid == 0);
}


