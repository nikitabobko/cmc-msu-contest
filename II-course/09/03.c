#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int mysys(const char *str) {
    pid_t pid = fork();
    if (!pid) {
        execlp("sh", "sh", "-c", str, NULL);
        _exit(-1);
    } else if (pid < 0) {
        return -1;
    }
    int status;
    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status) >= 0 && WEXITSTATUS(status) <= 127) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return WTERMSIG(status) + 128;
    }
    return -1;
}
