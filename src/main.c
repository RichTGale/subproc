/**
 * main.c
 *
 * This file demonstrates the use of the subproc type.
 * 
 * The subproc type launches a sub-process and uses it to execute a shell
 * command.
 *
 * Author: Richard Gale
 * Version: 1.0.1
 */

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "mycutils.h"
#include "subproc.h"

/* This is the number of nanoseconds the subproc will run for. */
#define SPROC_RUN_TIME   NANOS_PER_SEC * (uint64_t) 10

/* This is the number of nanoseconds to wait between printing
 * status messages. */
#define STATUS_FREQ_TIME NANOS_PER_SEC * (uint64_t) 1

/**
 * This is the program's main function.
 */
int main (int argc, char* argv[])
{
    subproc sp; /* The sub-process. */

    /* The time at which the subproc was executed. */
    struct timespec sp_start;

    /* The time at which the subproc's status was last printed to the user. */
    struct timespec last_status;

    char* tstamp;   /* A time stamp. */
    bool running = true;    /* Whether the loop should loop. */

    /* Initialise the subprocess and use it to execute a shell command. */
    subproc_init(&sp);
    subproc_exec(&sp, "ls", "./output/");
    start_timer(&sp_start);

    /* subproc_exec() will have printed a status message. */
    start_timer(&last_status);

    /* Run the processes. */
    while (running)
    {
        /* Check if the subproc should be terminated. */
        if (check_timer(sp_start, SPROC_RUN_TIME))
        {
            /* Print a status message. */
            fprintf( stdout,
                    "[ %s ] Sub-process is being terminated...\n", 
                    (tstamp = timestamp()));

            /* Terminate the subprocess. */
            subproc_term( &sp );

            /* Destroy the subproc. */
            subproc_free( &sp );

            /* Stop the loop from looping. */
            running = false;
        }

        /* Check if the user should be updated as to the status of
         * the subproc. */
        else if (check_timer(last_status, STATUS_FREQ_TIME))
        {
            /* Print a status message. */
            fprintf(stdout, 
                    "[ %s ] Sub-process is running...\n",
                    (tstamp = timestamp()));
 
            /* Reset the timer. */
            start_timer(&last_status); 
        }
    }

    /* Exit the program. */
    exit(EXIT_SUCCESS);
}
