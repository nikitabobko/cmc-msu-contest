#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

enum 
{
    BUF_SIZE = 8,
    CHILDREN_NUM = 3
};

int main(void) {
    char buf[BUF_SIZE];
    setbuffer(stdin, buf, sizeof(buf));
    for (int i = 1; i <= CHILDREN_NUM; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int num;
            scanf("%d", &num);
            
            printf("%d %d\n", i, num*num);
            return 0;
        } else if (pid < 0) {
            return 1;
        }
    }
    for (int i = 0; i < CHILDREN_NUM; i++) {
        wait(NULL);
    }
}