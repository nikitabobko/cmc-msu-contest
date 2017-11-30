#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int run_process(const char *cmd, int in_fd, int out_fd, int parallel) {
    pid_t pid;
    if (!(pid = fork())) {
        if (in_fd != -1) {
            dup2(in_fd, 0);
        }
        if (out_fd != -1) {
            dup2(out_fd, 1);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        exit(1);
    }
    if (parallel) {
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && !WEXITSTATUS(status);
    }
}

int main(int argc, char **argv) {
    if (argc != 6) {
        return 0;
    }
    int fd[2];
    pipe(fd);
    
    pid_t pid = fork();
    if (!pid) {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        
        exit(run_process(argv[1], -1, -1, 0) && 
            run_process(argv[2], -1, -1, 0));
    } else if (pid < 0) {
        return 1;
    }
    
    pid = fork();
    if (!pid) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        
        int out = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0664);
        if (out < 0) {
            exit(1);
        }
        dup2(out, 1);
        close(out);
        
        run_process(argv[3], -1, -1, 0);
        run_process(argv[4], -1, -1, 0);
    } else if (pid < 0) {
        return 1;
    }
    
    close(fd[0]);
    close(fd[1]);
    
    wait(NULL);
    wait(NULL);
    
    return 0;
}