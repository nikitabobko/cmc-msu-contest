#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    for (int i = 1; i < 3; i++) {
        pid_t pid = fork();
        if (pid > 0) {
            wait(NULL);
            printf("%d%c", i, i == 1 ? '\n' : ' ');

            return 0;
        } else if (pid < 0) {
            return 1;
        }
    }
    printf("%d ", 3);
    return 0;
}