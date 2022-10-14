/**
 * mycutils.c
 *
 * This file contains the definitions of various utility functions.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
 */

#include "mycutils.h"

/**
 * Obtains the current time, storing it in a timespec.
 */
void start_timer( struct timespec* ts )
{
    // Obtaining the current time.
    if ( (clock_gettime( CLOCK_REALTIME, ts) ) == -1 )
    {
        fprintf( stderr, 
                "[ %s ] ERROR: in function start_timer(): clock_gettime() "
                "returned an error: %s",
                timestamp(), strerror( errno ) );
    }
}

/**
 * Returns true if the provided wait-time has elapsed since the
 * provided timespec was given a time.
 */
bool check_timer( struct timespec start, uint64_t wait_time )
{
    struct timespec current; // The current time.
    struct timespec elapsed; // The elapsed time
    bool wait_has_elapsed = false; // Whether the designated time has elapsed.

    // Obtaining the current time.
    clock_gettime( CLOCK_REALTIME, &current );

    // Calculating the elapsed time.
    elapsed.tv_sec = current.tv_sec - start.tv_sec;
    elapsed.tv_nsec = current.tv_nsec - start.tv_nsec;

    // Determining whether the disignated time has elapsed.
    if ( ( elapsed.tv_sec * NANOS_PER_SEC ) + elapsed.tv_nsec >= wait_time )
    {
        wait_has_elapsed = true; // The designated time has elapsed.
    }

    // Retuning whether the designated time has elapsed.
    return wait_has_elapsed;
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
        if ( ( *str )[i] == remove )
        {
            // Setting the source and destinations points for moving.
            src = &( ( *str )[i + 1] );
            dst = &( ( *str )[i] );

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
    ( *str )[total_chars] = '\0';
}

/**
 * Returns a timestamp.
 */
char* timestamp()
{
    time_t current_time;
    char* c_time_string;

    // Obtaining the current time.
    current_time = time( NULL );
    if ( current_time == ( ( time_t ) - 1 ) )
    {
        // There was an error obtaining the time so we're printing 
        // a message and exiting.
        fprintf( stderr, "Failure to obtain the current time.\n" );
        exit( EXIT_FAILURE );
    }

    // Converting time to local time format.
    c_time_string = ctime(&current_time);
    if ( c_time_string == NULL )
    {
        // There was an error converting the time to a string so we're
        // printing a message and exiting.
        fprintf( stderr, "Failure to convert the current time.\n" );
        exit( EXIT_FAILURE );
    }

    // Removing the newline character that was added by ctime().
    rmchar( &c_time_string, '\n' );

    return c_time_string;
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
        fprintf( stderr, "[ %s ] ERROR: In open_file(): "
                    "Could not open file %s\n", timestamp(), fname );
        exit( EXIT_FAILURE );
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
        fprintf( stderr, "[ %s ] Error closing file\n", timestamp() );
        exit( EXIT_FAILURE );
    }
}
