#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int run_process(const char *cmd) {
    pid_t pid;
    if (!(pid = fork())) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        exit(1);
    }
    int status;
    wait(&status);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int main(int argc, const char **argv) {
    if (argc != 4) {
        return 1;
    }
    return !((run_process(argv[1]) || run_process(argv[2])) && run_process(argv[3]));
}