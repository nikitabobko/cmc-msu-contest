#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

static int flag = 0;

void handler(int sig) {
    flag = 1;
    signal(SIGUSR1, &handler);
}

void ping_pong(FILE *w, FILE *r, int child_num, pid_t bro_pid, int max) {
    if (child_num == 2) {
        while(!flag) {
            usleep(1000);
        }
        flag = 0;
    }
    int num;
    while(1) {
        if (fscanf(r, "%d", &num) != 1 || num == 0) {
            break;
        }
        
        if (num >= max) {
            break;
        }

        printf("%d %d\n", child_num, num++);
        fflush(stdout);
        
        if (num >= max) {
            break;
        }

        fprintf(w, "%d\n", num);
        fflush(w);
        
        kill(bro_pid, SIGUSR1);
        
        while(!flag) {
            usleep(1000);
        }
        flag = 0;
    }
    fprintf(w, "0\n");
    fflush(w);
    fclose(w);
    fclose(r);
    kill(bro_pid, SIGUSR1);
    exit(0);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int max = strtol(argv[1], NULL, 10);

    int fd[2];
    pipe(fd);
    FILE *r = fdopen(fd[0], "r");
    FILE *w = fdopen(fd[1], "w");

    signal(SIGUSR1, &handler);
    pid_t pid = fork();
    if (!pid) {
        pid_t bro_pid;
        fscanf(r, "%d", &bro_pid);
        ping_pong(w, r, 1, bro_pid, max);
    }
    pid_t pid2 = fork();
    if (!pid2) {
        ping_pong(w, r, 2, pid, max);
    }
    fprintf(w, "%d\n", pid2);
    fflush(w);

    int num = 1;
    fprintf(w, "%d\n", num);
    fflush(w);

    fclose(w);
    close(fd[0]);
    fclose(r);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    printf("Done\n");
    return 0;
}