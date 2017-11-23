#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int run_process(const char *cmd, int input_fd, int output_fd, int parallel) {
    if (input_fd != -1) {
        dup2(input_fd, fileno(stdin));
    }
    if (output_fd != -1) {
        dup2(output_fd, fileno(stdout));
    }
    pid_t pid;
    if (!(pid = fork())) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        exit(0);
    }
    if (!parallel) {
        int status;
        wait(&status);
        return WIFEXITED(status) && !WEXITSTATUS(status);
    } else {
        return 1;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 6) {
        return 0;
    }
    const char *cmd1 = argv[1];
    const char *cmd2 = argv[2];
    const char *cmd3 = argv[3];
    const char *file1_name = argv[4];
    const char *file2_name = argv[5];
    
    int fd[2];
    if (pipe(fd) < 0) {
        return 0;
    }

    pid_t pid = fork();
    if (!pid) {
        close(fd[0]);
        if (dup2(fd[1], fileno(stdout)) < 0) {
            exit(1);
        }

        int in = open(file1_name, O_RDONLY);
        if (in < 0) {
            exit(1);
        }

        exit(run_process(cmd1, in, -1, 0) && run_process(cmd2, -1, -1, 0));
    } else if (pid < 0) {
        return 0;
    }
    close(fd[1]);

    int out = open(file2_name, O_APPEND | O_CREAT | O_WRONLY, 0664);
    if (out < 0) {
        wait(NULL);
        return 0;
    }
    run_process(cmd3, fd[0], out, 1);
    close(fd[0]);
    close(out);
    
    wait(NULL);
    wait(NULL);
    return 0;
}