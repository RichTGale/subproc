/**
 * main.c
 *
 * This file demonstrates the use of the subproc type.
 * The subproc type launches a sub-process and uses it to 
 * execute a shell command.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
 */

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "mycutils.h"
#include "subproc.h"

#define SPROC_RUN_TIME NANOS_PER_SEC * ( uint64_t ) 10
#define BLOCK_RUN_FREQ NANOS_PER_SEC * ( uint64_t ) 1

int main (int argc, char* argv[])
{
    subproc sp; // The sub-process.
    struct timespec ts_sp; // timespec for timing the sub-process.
    struct timespec ts_out; // Times output for the user about whether the sub-process is running.
    bool running = true; // Whether the loop should loop.

    // Initialising the subprocess and using it to execute a
    // shell command.
    subproc_init( &sp );
    subproc_exec( &sp, "ls", "../output/" );

    // Setting the timespecs with the current time.
    start_timer( &ts_sp );
    start_timer( &ts_out );

    // Running the processes.
    while ( running )
    {
        if ( check_timer( ts_sp, SPROC_RUN_TIME ) )
        {
            // Telling the user that the subprocess is going to be terminated.
            fprintf( stdout, "[ %s ] Sub-process is being terminated...\n", 
                    timestamp() );
            subproc_term( &sp );    // Terminating the subprocess.
            subproc_free( &sp );    // Freeing memory allocated to the subproc.
            running = false;
        }
        else if ( check_timer( ts_out, BLOCK_RUN_FREQ ) )
        {
            // Telling the user that the subprocess is still running.
	        fprintf( stdout, 
                    "[ %s ] Sub-process is running...\n", timestamp() );
	        start_timer( &ts_out ); // Restarting the timer. 
        }
    }

    exit( EXIT_SUCCESS );
}
