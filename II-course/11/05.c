#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

enum 
{
    SIG_INT_COUNT_TO_EXIT = 4
};

volatile int last_found_prime = 0;
int sig_int_count = 0;

void hndl(int sig) {
    if (sig == SIGTERM) {
        exit(0);
    } else { // sig == SIGINT
        sig_int_count++;
        if (sig_int_count == SIG_INT_COUNT_TO_EXIT) {
            exit(0);
        }
        printf("%d\n", last_found_prime);
        fflush(stdout);
        signal(SIGINT, &hndl);
    }
}

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i*i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char const *argv[]) {
    signal(SIGTERM, &hndl);
    signal(SIGINT, &hndl);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int low, high;
    scanf("%d %d", &low, &high);

    for (int i = low; i < high; i++) {
        if (is_prime(i)) {
            last_found_prime = i;
        }
    }

    printf("-1\n");
    fflush(stdout);

    return 0;
}