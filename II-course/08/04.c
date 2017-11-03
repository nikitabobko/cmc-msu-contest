#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        printf("%d%c", i, i == n ? '\n' : ' ');
        fflush(stdout);
        pid_t pid = fork();
        if (pid > 0) {
            while (wait(NULL) != -1);
            return 0;
        } else if (pid < 0) {
            return 1;
        }
    }
    return 0;
}