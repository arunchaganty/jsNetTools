
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

