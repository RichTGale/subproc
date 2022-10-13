

#ifndef MYCUTILS_H
#define MYCUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Opens the file with the provided file name in the provided mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file( char* fname, char* mode );

/**
 * Closes the provided file stream. If there is an error, it is printed on
 * stderr and the program will exit.
 */
void close_file( FILE* fp );

/**
 * Removes all cases of the provided char from the string at the
 * provided pointer.
 */
char* rmchar( char** str, char remove );

#endif //MYCUTILS
