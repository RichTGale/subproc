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

#include "mycutils.h"
#include "subproc.h"

int main (int argc, char* argv[])
{
    subproc s;

    subproc_init( &s );

    subproc_run( &s, "ls", 1 );

    subproc_free( &s );

    exit( EXIT_SUCCESS );
}
