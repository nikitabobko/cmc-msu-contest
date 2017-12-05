#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

enum 
{
    KEY = 42352
};

void kill_children(pid_t *pids, int nproc) {
    if (pids == NULL) {
        return;
    }
    for (int i = 0; i < nproc; i++) {
        kill(pids[i], SIGTERM);
    }
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        return 0;
    }
    setbuf(stdin, NULL);
    int count = strtol(argv[1], NULL, 10);
    
    pid_t *pids = malloc(count * sizeof(*pids));
    int sem_id = -1;
    if (pids == NULL) {
        goto free_res;
    }

    sem_id = semget(KEY, count, 0666 | IPC_CREAT);

    if (sem_id < 0) {
        goto free_res;
    }

    for (int i = 0; i < count; i++) {
        pids[i] = fork();
        if (!pids[i]) {
            while (1) {
                struct sembuf op = {i, -1, SEM_UNDO};
                if (semop(sem_id, &op, 1) < 0) {
                    exit(1);
                }
                
                int num;
                if (scanf("%d", &num) != 1) {
                    exit(0);
                }
                printf("%d %d\n", i, num);
                fflush(stdout);
                if (num < 0) {
                    num %= count;
                    num += count;
                }

                struct sembuf op1 = {num % count, 1, SEM_UNDO};
                if (semop(sem_id, &op1, 1) < 0) {
                    exit(1);
                }
            }
        } else if (pids[i] < 0) {
            goto free_res;
        }
    }
    struct sembuf op = {0, 1, SEM_UNDO};
    if (semop(sem_id, &op, 1) < 0) {
        goto free_res;
    }
    wait(NULL);

free_res:
    kill_children(pids, count);
    free(pids);
    if (sem_id != -1) {
        semctl(sem_id, count, IPC_RMID);
    }
    return 0;
}