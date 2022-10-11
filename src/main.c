#include <stdlib.h>

#include "subproc.h"

int main (int argc, char* argv[])
{
    subproc s;

    subproc_init( &s );

    subproc_run( &s, "ls", 1 );

    subproc_free( &s );

    exit( EXIT_SUCCESS );
}
