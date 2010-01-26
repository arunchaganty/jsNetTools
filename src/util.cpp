/**
 * @file util.cpp
 * @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
 * @version 0.1
 * 
 * @section Description
 * Assorted generic utility functions
 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

/**
 * Sets up a pipe for output and spawns a process.
 *
 * @param path - Path to executable
 * @param args - Command line arguments
 * @param pipe_fd - Created file descriptors for pipe will be stored in this array
 * @return - PID of spawned process.
 */
pid_t spawn(const char* path, const char* args[], int pipe_fd[2])
{
    pid_t pid;
    if(pipe(pipe_fd) == -1)
    {
        return;
    }
    
    pid = fork();
    if(pid == -1)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    }
    else if(pid == 0)
    {
        /* Child Process */
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        if( execv( path, args) == -1 )
        {
            exit(-1); 
        }
    }

    return pid;
}

