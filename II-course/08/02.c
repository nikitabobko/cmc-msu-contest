#include <stdio.h>
#include <unistd.h>
#include <wait.h>

enum 
{
    CHILDREN_NUM = 2,
};

int main(void) {
    for (int i = 1; i <= CHILDREN_NUM; i++) {
        pid_t pid = fork();
        if (pid > 0) {
            wait(NULL);
            printf("%d%c", i, i == 1 ? '\n' : ' ');

            return 0;
        } else if (pid < 0) {
            return 1;
        }
    }
    printf("%d ", CHILDREN_NUM + 1);
    return 0;
}