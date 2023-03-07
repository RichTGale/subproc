/**
 * subproc.h
 *
 * This file contains the publicly available data-structure and
 * procedure declarations for the subproc type.
 * The subproc type asyncronously executes shell commands as a 
 * sub/child process.
 *
 * Author: Richard Gale
 * Version: 14th October, 2022
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
 * The subproc data-structure.
 */
typedef struct subproc_data* subproc;

/**
 * Allocates memory to the provided subproc.
 */
void subproc_init(subproc* sp);

/**
 * Frees the memory allocated to the subproc.
 */
void subproc_free(subproc* sp);

/**
 * Executes the provided command as a sub-process and returns its PID.
 */
void subproc_exec( subproc* sp, char* cmd, char* fdir );

/**
 * Requests for the provided sub-process to be terminated, waits for it to
 * exit, and reports its exit-status to stdout or stderr if there was an error.
 */
void subproc_term( subproc* sp );

#endif // SUBPROC_H
