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

/* Definitions for timers. */
#define SPROC_RUN_TIME NANOS_PER_SEC * (uint64_t) 10
#define BLOCK_RUN_FREQ NANOS_PER_SEC * (uint64_t) 1

int main (int argc, char* argv[])
{
    subproc sp;             /* The sub-process. */
    struct timespec ts_sp;  /* Timer for the the sub-process. */
    struct timespec ts_out; /* Timer for output about whether the sub-process is running. */
    char* tstamp;           /* A time stamp. */
    bool running = true;    /* Whether the loop should loop. */

    /* Initialise the subprocess and use it to execute a shell command. */
    subproc_init(&sp);
    subproc_exec(&sp, "ls", "./output/");

    /* Set the timespecs with the current time. */
    start_timer(&ts_sp);
    start_timer(&ts_out);

    /* Running the processes. */
    while (running)
    {
        /* Check if the subproc should be terminated. */
        if (check_timer(ts_sp, SPROC_RUN_TIME))
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
        else if (check_timer( ts_out, BLOCK_RUN_FREQ))
        {
            /* Print a status message. */
            fprintf(stdout, 
                    "[ %s ] Sub-process is running...\n",
                    (tstamp = timestamp()));
 
            /* Reset the timer. */
            start_timer(&ts_out); 
        }
    }

    /* Exit the program. */
    exit(EXIT_SUCCESS);
}
