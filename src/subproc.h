/**
 * subproc.h
 *
 * This file contains the publicly available data-structure and function
 * prototype declarations for the subproc type.
 * 
 * The subproc type executes shell commands as a sub/child process.
 *
 * Author: Richard Gale
 * Version: 1.0.1
 */

#ifndef SUBPROC_H
#define SUBPROC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "mycutils.h"

/**
 * This is the subproc data-structure.
 */
typedef struct subproc_data* subproc;

/**
 * This function initialises the subproc provided to it.
 */
void subproc_init(subproc* sp);

/**
 * This function destroys the subproc provided to it.
 */
void subproc_free(subproc* sp);

/**
 * This function executes the command provided to it as a sub-process.
 */
void subproc_exec( subproc* sp, char* cmd, char* fdir );

/**
 * This function requests for the provided sub-process to be terminated, waits
 * for it to exit, and reports its exit-status to stdout or stderr if there
 * was an error.
 */
void subproc_term( subproc* sp );

#endif // SUBPROC_H
