/**
 * main.c
 *
 * This file demonstrates the use of the subproc type.
 * The subproc type launches a sub-process and uses it to 
 * execute a shell command.
 *
 * Author: Richard Gale
 * Version: 12th October, 2022
 */

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "mycutils.h"
#include "subproc.h"

int main (int argc, char* argv[])
{
    subproc sp;
    struct timespec ts_sp;
    struct timespec ts_out;
    bool running = true;

    subproc_init( &sp );
    subproc_exec( &sp, "ls" );
    start_timer( &ts_sp );
    start_timer( &ts_out );

    while ( running )
    {
        if ( !( check_timer( ts_sp, NANOS_PER_SEC * ( uint64_t) 10 ) ) )
        {
            if ( check_timer( ts_out, NANOS_PER_SEC * ( uint64_t) 1 ) )
            {
                timestamp( stdout );
                fprintf( stdout, "Sub-process is running...\n" );
                start_timer( &ts_out );
            }
        }
        else
        {
            timestamp( stdout );
            fprintf( stdout, "Sub-process is being terminated...\n" );
            subproc_term( &sp );
            subproc_free( &sp );
            running = false;
        }
    }

    exit( EXIT_SUCCESS );
}
