/*
Submit a solution for up12-1-unix/ipc/sem-1
Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	2 s
Memory limit:	64M
Open date:	2017/12/05 10:30:00
Date penalty formula:	2017/12/31 0-50/14d
Next soft deadline:	2017/12/19 10:30:00
Problem up12-1: unix/ipc/sem-1
Программе в аргументах командной строки задаются:

Количество процессов (nproc);
Ключ IPC (key);
Максимальное значение (maxval).
Программа должна создать nproc процессов и запустить между ними пересылку числа. Каждый процесс получает число, печатает свой порядковый номер (от 0) и значение числа, как показано в пример, увеличивает его на 1 и пересылает число процессу с номером (val * val) % nproc, где val – значение числа после увеличения. Если значение числа после увеличения стало больше maxval, процесс не пересылает число следующему, а (совместно с родителем) выполняет действия по завершению игры. Игра начинается с первого из созданных процессов со значения 0. Отец дожидается завершения всех процессов и сам завершает работу.

Обратите внимание, при достижении максимального значения оно печатается, но не пересылается дальше.

Для синхронизации процессов используйте семафоры. Для пересылки значения используйте разделяемую память. Массив семафоров и разделяемая память должны уничтожаться в конце работы.

Тестирование завершается с вердиктом 'Synchronization error', если процесс-отец (то есть ваша программа, запускаемая на тестирование) заканчивает работу раньше какого-либо из своих потомков.

Тестирование завершается с вердиктом 'Security violation', если после завершения работы вашей программы остались неудаленные объекты IPC.

Examples
Input
Output
0: 0
1: 1
*/
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
    volatile long long *mem;
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