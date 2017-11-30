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

int run_process(const char *cmd, int input_fd, int output_fd, int parallel, pid_t *child_pid) {
    pid_t pid;
    if (!(pid = fork())) {
        if (input_fd != -1) {
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                exit(1);
            }
            close(input_fd);
        }
        if (output_fd != -1) {
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                exit(1);
            }
            close(output_fd);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        exit(0);
    }
    if (child_pid) {
        *child_pid = pid;
    }
    if (!parallel) {
        int status;
        waitpid(pid, &status, 0);
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
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
            exit(1);
        }
        close(fd[1]);

        int in = open(file1_name, O_RDONLY);
        if (in < 0) {
            exit(1);
        }

        exit(run_process(cmd1, in, -1, 0, NULL) && run_process(cmd2, -1, -1, 0, NULL));
    } else if (pid < 0) {
        return 0;
    }
    close(fd[1]);

    int out = open(file2_name, O_APPEND | O_CREAT | O_WRONLY, 0664);
    if (out < 0) {
        close(fd[0]);
        wait(NULL);
        return 0;
    }
    pid_t cmd3_pid;
    run_process(cmd3, fd[0], out, 1, &cmd3_pid);
    close(fd[0]);
    close(out);

    wait(NULL);
    wait(NULL);
    return 0;
}