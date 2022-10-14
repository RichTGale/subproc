
#include "mycutils.h"

#define FILE_OPEN_ERROR 1
#define FILE_CLOSE_ERROR 2

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
 * Prints a timestamp to the provided file-stream.
 */
void timestamp( FILE* stream )
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

    // Printing the time to the file stream.
    fprintf( stream, "[ %s ] ", c_time_string);
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
        timestamp( stderr );
        fprintf( stderr, "ERROR: In open_file(): "
                    "Could not open file %s\n", fname );
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
        timestamp( stderr );
        fprintf( stderr, "Error closing file\n" );
        exit( FILE_CLOSE_ERROR );
    }
}