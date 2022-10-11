/**
 * subproc.c
 *
 * This file contains the internal data and procedure definitions for the 
 * subproc type.
 * The subproc type asyncronously executes shell commands as a 
 * sub/child process.
 *
 * Author: Richard Gale
 * Version: 12th October, 2022
 */

#include "subproc.h"

#define FILE_OPEN_ERROR 1
#define FILE_CLOSE_ERROR 2
#define DUP_ERROR 3 
#define PIPE_ERROR 4
#define FORK_ERROR 5

/**
 * The internal data of the subproc data-structure.
 */
struct subproc_data {
    int fds[2]; // File descriptors.
    pid_t pid;  // Process Id
};

/**
 * Allocates memory to the provided subproc.
 */
void subproc_init(subproc* sp)
{
    *sp = (subproc) malloc( sizeof( struct subproc_data ) );
}

/**
 * Frees the memory allocated to the subproc.
 */
void subproc_free(subproc* sp)
{
    free(*sp);
}

/**
 * Opens the file with the provided file name in the provided mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file( char* fname, char* mode )
{
    FILE* fp;   // The pointer to the file stream.

    // Attempting to open the file.
    if ( ( fp = fopen( fname, mode ) ) == NULL )
    {
        // There was an error opening the file.
        fprintf( stderr, "ERROR: In open_file(): "
                    "Could not open file %s", fname );
        exit( FILE_OPEN_ERROR );
    }

    // Returning the pointer to the file stream.
    return fp;
}

/**
 * Closes the provided file stream. If there is an error, it is printed on
 * stderr and the program will exit.
 */
void close_file( FILE* fp )
{
    // Attempting to close the file stream.
    if ( fclose( fp ) != 0 )
    {
        // There was an error closing the file stream.
        fprintf( stderr, "Error closing file" );
        exit( FILE_CLOSE_ERROR );
    }
}

/**
 * Calls dup2() using the provided file-descriptors for its actual parameters.
 * If there is an error, it is printed on stderr and the program exits.
 *
 * dup2() is used to specify the file descriptor being used for a pipe(),
 * file stream, etc...
 */
void duperr( int fdold, int fdnew )
{
    // Attempting to duplicate the file descriptor.
    if ( dup2( fdold, fdnew ) == -1 ) 
    {    
        // There was an error.
        fprintf( stderr, "dup2 failed on fileno() %s", strerror( errno ) );
        exit ( DUP_ERROR );
    }
}

/**
 * Removes all cases of the provided char from the string at the
 * provided pointer.
 */
char* rmchar( char** str, char remove )
{
    int len = strlen( *str );   // The original length of the string.
    int total_chars = strlen( *str );   // The current length of the string.
    int i; // An indexer
    char* src; // The address of where to start moving the memory.
    char* dst; // The address of where to move the memory to.

    // Overwriting the unwanted character.
    for ( i = 0; i < len; i++ )
    {
        if ( (*str)[i] == remove )
        {
            // Setting the source and destinations points for moving.
            src = &( (*str)[i + 1] );
            dst = &( (*str)[i] );

            // Overwriting an unwanted character.
             memmove(dst, src, ( sizeof(char) * strlen( *str ) ) - 
                                    ( sizeof( char ) * i ) );

            // Decrementing the index so we will check the 
            // replacement character.
            i--;

            // Recording the new length of the string.
            total_chars--;
        }
    }

    // Designating the end of the string.
    (*str)[total_chars] = '\0';
}

/**
 * Concatenates three strings.
 */
void mkfname(char** ptr, char* dir, char* body, char* ext)
{
    // A copy of the body
    char* body_cpy;

    // Allocating memory for the copy
    body_cpy = ( char* ) malloc( sizeof( char ) * strlen( body ) );

    // Copying the command
    strcpy( body_cpy, body );

    // Removing unwanted characters from the copy
    rmchar( &body_cpy, '/' );
    rmchar( &body_cpy, '.' ); 

    // Building the file name
    strcat( *ptr, dir );
    strcat( *ptr, body_cpy);
    strcat( *ptr, ext );

    // Unallocating memory for the copy
    free( body_cpy );
}

/**
 * Executes the provided command as a sub-process and returns its PID.
 */
void subproc_exec( subproc* sp, char* cmd )
{
    FILE* fout; // File for stdout
    FILE* ferr; // File for stderr
    char* fname_out; // Output file name for stdout.
    char* fname_err; // Output file name for stderr.
    // The relative path of the directory to save the output files in.
    char* fdir = "../";
    // The file extensions
    char* fext_out = "_out.txt";
    char* fext_err = "_err.txt";

    // Attempting to create a pipe to use for the child process.
    if ( pipe( ( *sp )->fds ) == -1 )
    {
        // There was an error creating the pipe so we are printing it on
        // and exiting the program.
        fprintf( stderr, "ERROR: In exec_child_process(): "
                            "pipe() - %s", strerror( errno ) );
        exit( PIPE_ERROR );
    }

    // Attempting to create the child process.
    if ( ( ( *sp )->pid = fork() ) == -1 )
    {
        // There was an error creating the child process so we are printing
        // it and exiting the program.
        fprintf( stderr, "ERROR: In exec_child_process: "
                            "fork() - %s", strerror( errno ) );
        exit( FORK_ERROR );
    }
    else if ( ( *sp )->pid == 0 )    // The child process
    {
        // Printing status message.
        fprintf( stdout, "Creating sub-process...\n" );

        // Allocating memory to the output file name strings.
        fname_out = ( char* ) malloc( sizeof( char ) * 
                    ( strlen( fdir ) + strlen( cmd ) + strlen( fext_out ) ) );
        fname_err = ( char* ) malloc( sizeof( char ) * 
            ( strlen( fdir ) + strlen( cmd ) + strlen( fext_err ) ) );


        // Creating file names for stdout and stderr
        mkfname(&fname_out, "../output/", cmd, "_out.txt");
        mkfname(&fname_err, "../output/", cmd, "_err.txt");

        // Setting the read descriptor of the child process to 
        // be stdin's descriptor.
        duperr( ( ( *sp )->fds )[0], STDIN_FILENO );
	    
        // Opening file streams.
        fout = open_file( fname_out, "w" );
        ferr = open_file( fname_err, "w" );
        
        // Setting the descriptor of the output file's stream to 
        // be stdout's descriptor.
        duperr( fileno( fout ), STDOUT_FILENO );

        // Setting the descriptor of the error file's stream to 
        // be stderr's descriptor.
        duperr( fileno( ferr ), STDERR_FILENO );

        // Now that the descriptors have been set we can close the 
        // file streams.
        close_file( fout );
        close_file( ferr );

        free( fname_out );
        free( fname_err );

        // Printing status message.
        fprintf( stdout, "Sub-process created... Executing command...\n" );

        // Executing the command as the child process.
        execl ( "/bin/sh", "sh", "-c", cmd, NULL );
        
        // There was an error executing the command so we are printing it.
        fprintf( stderr, "execl failed with error - %s", strerror( errno ) );
    }
}

/**
 * Requests for the provided sub-process to be terminated, waits for it to
 * exit, and reports its exit-status to stdout or stderr if there was an error.
 */
void subproc_term( subproc* sp )
{
    int status; // The exit status of the process.

    // Printing status message.
    fprintf( stdout, "Terminating sub-process...\n" );

    // Terminating the process.
    kill( ( *sp )->pid, SIGTERM);

    // Waiting for the process to exit and then printing its status.
    do
    {
        if ( ( ( *sp )->pid = waitpid( ( *sp )->pid, &status, WNOHANG ) ) == -1 )
        {
            // There was an error waiting for the process to exit.
            fprintf( stderr, "ERROR: in term_child_process(): wait() error!\n");
        }
        else if ( ( *sp )->pid == 0 )
        {
            // The process is still running.
            fprintf( stdout, "Waiting for process to terminate...\n");
            sleep ( 1 );
        }
        else
        {
            // There is no longer a process with the pid so we'll look at
            // what happened to it.
            if ( WIFEXITED( status ) )
            {
                // The process exited normally so we are priting its
                // exit status.
                fprintf( stdout, "The process exited normally "
                        "with exit status %d.\n", WEXITSTATUS( status ) );
            }
            else if ( WIFSIGNALED( status ) )
            {
                // The process exited because of an uncaught signal.
                fprintf( stdout, "The process did not exit normally");
            }
            else
            {
                // The process did not exit.
                fprintf( stdout, "The child process did not exit\n");
            }
        }
    } while ( ( *sp )->pid == 0 );
}

/**
 * Executes the provided command as a sub-process before terminating
 * the sub-process after the provided run-time. If the privided run-time
 * is zero (0) then the process is terminated immediately.
 */
void subproc_run( subproc* sp, char* cmd, unsigned int runtime )
{
    // Executing the command as a sub-process.
    subproc_exec( sp, cmd );

    // Letting the process run for a duration.
    if ( runtime > 0 )
        sleep( runtime );
   
    // Printing status message.

    // Terminating the sub-process.
    subproc_term( sp );
}

