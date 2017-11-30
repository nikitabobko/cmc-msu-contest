#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>

void kill_children(pid_t *children_pids, int children_count) {
    for (int i = 0; i < children_count; i++) {
        if (children_pids[i] > 0) {
            kill(children_pids[i], SIGKILL);
        } else {
            break;
        }
    }
    for (int i = 0; i < children_count; i++) {
        wait(NULL);
    }
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        return 0;
    }
    int stdin_fd_backup = dup(STDIN_FILENO);
    if (stdin_fd_backup < 0) {
        return 1;
    }
    pid_t *children_pids = calloc((argc - 1), sizeof(*children_pids));
    if (!children_pids) {
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        int fd[2];
        if (i != argc - 1 && pipe(fd) < 0) {
            kill_children(children_pids, argc - 1);
            return 1;
        }

        children_pids[i - 1] = fork();
        if (!children_pids[i - 1]) {
            if (i != argc - 1 && (dup2(fd[1], STDOUT_FILENO) < 0 || close(fd[1]) < 0 || 
                    close(fd[0]) < 0)) {
                exit(1);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else if (children_pids[i - 1] < 0) {
            kill_children(children_pids, argc - 1);
            return 1;   
        }

        if (i != argc - 1 && (dup2(fd[0], 0) < 0 || close(fd[0]) < 0 || 
                close(fd[1]) < 0)) {
            kill_children(children_pids, argc - 1);
            return 1;
        }
    }
    if (dup2(stdin_fd_backup, 0) < 0 || close(stdin_fd_backup) < 0) {
        kill_children(children_pids, argc - 1);
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }
    
    free(children_pids);
    return 0;
}