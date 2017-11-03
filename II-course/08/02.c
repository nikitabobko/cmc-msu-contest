#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    for (int i = 1; i < 3; i++) {
        pid_t pid = fork();
        if (pid > 0) {
            while (wait(NULL) != -1);
            if (i == 1) {
                printf("%d\n", i);
            } else {
                printf("%d ", i);
            }

            return 0;
        } else if (pid < 0) {
            return 1;
        }
    }
    printf("%d ", 3);
    return 0;
}