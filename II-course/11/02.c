#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef enum Mode {
    Mode1, Mode2
} Mode;

static Mode mode = Mode1;

void sighandler(int sig) {
    if (sig == SIGUSR1) {
        mode = Mode1;
    } else {
        mode = Mode2;
    }
    signal(SIGUSR1, &sighandler);
    signal(SIGUSR2, &sighandler);
}

int main(int argc, char const *argv[]) {
    signal(SIGUSR1, &sighandler);
    signal(SIGUSR2, &sighandler);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int num;
    while (scanf("%d", &num) == 1) {
        if (mode == Mode1) {
            printf("%d\n", -num);
        } else if (mode == Mode2) {
            printf("%d\n", num * num);
        }
        fflush(stdout);
    }
    return 0;
}