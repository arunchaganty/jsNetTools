/**
 * @file util.h
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Assorted generic utility functions
 */

#ifndef UTIL_H
#define UTIL_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ARR_LEN(arr) ((int)(sizeof(arr)/sizeof(arr[0])))
#define DBG_PRINTV(fmt, ...)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__, __VA_ARGS__))
#define DBG_PRINT(fmt)     (fprintf(stderr, "%s:" fmt "\n", __FUNCTION__))

/**
 * Sets up a pipe for output and spawns a process.
 *
 * @param path - Path to executable
 * @param args - Command line arguments
 * @param pipe_fd - Created file descriptors for pipe will be stored in this array
 * @return - PID of spawned process.
 */
pid_t spawn(const char* path, const char* args[], int pipe_fd[2]);

#endif //UTIL_H

