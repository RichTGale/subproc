/**
 * subproc.h
 *
 * This file contains the publicly available data-structure and
 * procedure declarations for the subproc type.
 * The subproc type asyncronously executes shell commands as a 
 * sub/child process.
 *
 * Author: Richard Gale
 * Version: 9th October, 2022
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
 * Executes the provided command as a sub-process before terminating
 * the sub-process after the provided run-time. If the privided run-time
 * is zero (0) then the process is terminated immediately.
 */
void subproc_run(subproc* sp, char* CMD, unsigned int runtime );

#endif // SUBPROC_H