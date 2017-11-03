#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(void) {
    setbuf(stdin, NULL);
    int ret, num = 0;
    int i = 0;
    do {
        pid_t pid = fork();
        if (pid == 0) {
            ret = scanf("%d", &num);
        } else if (pid > 0) {
            int status;
            wait(&status);
            if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                if (i == 0) {
                    printf("-1");
                    return 0;
                }
                return 1;
            }
            if (i != 0) {
                printf("%d\n", num);
            }
            return 0;
        } else {
            return 1;
        }
        i++;
    } while (ret == 1);
    return 0;
}