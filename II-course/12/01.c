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

void kill_children(pid_t *pids, int nproc) {
    for (int i = 0; i < nproc; i++) {
        kill(pids[i], SIGTERM);
    }
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
}

int main(int argc, char const *argv[]) {
    if (argc < 4) {
        return 0;
    }
    int nproc = strtol(argv[1], NULL, 10);
    key_t key = strtoll(argv[2], NULL, 10);
    long long maxval = strtoll(argv[3], NULL, 10);

    int sem_id = semget(key, nproc, 0777 | IPC_CREAT);
    long long *mem;
    int mem_id = shmget(key, sizeof(*mem), 0666 | IPC_CREAT);

    if (sem_id < 0 || mem_id < 0) {
        return 1;
    }
    pid_t *pids = malloc(nproc * sizeof(*pids));
    if (!pids) {
        goto free_res;
    }
    mem = shmat(mem_id, NULL, 0);
    if (mem == (void *)-1) {
        goto free_res;
    }

    for (int i = 0; i < nproc; i++) {
        pids[i] = fork();
        if (!pids[i]) {
            while (1) {
                struct sembuf op = {i, -1, SEM_UNDO};
                if (semop(sem_id, &op, 1) < 0) {
                    exit(1);
                }
    
                if (mem == (void *)-1) {
                    exit(1);
                }
    
                printf("%d: %lld\n", i, (*mem)++);
                fflush(stdout);
    
                if (*mem > maxval) {
                    exit(0);
                }
    
                struct sembuf op1 = {((*mem) * (*mem)) % nproc, 1, SEM_UNDO};
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
    if (pids) {
        kill_children(pids, nproc);
    }
    free(pids);
    semctl(sem_id, nproc, IPC_RMID);
    shmctl(mem_id, IPC_RMID, 0);
    return 0;
}