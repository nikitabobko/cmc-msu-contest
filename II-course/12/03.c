/*
Submit a solution for up12-3-unix/ipc/ping-pong-c
Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    2 s
Memory limit:   64M
Open date:  2017/12/05 10:30:00
Date penalty formula:   2017/12/31 0-50/14d
Next soft deadline: 2017/12/19 10:30:00
Problem up12-3: unix/ipc/ping-pong-c
В аргументах командной строки программе задается одно целое число count - число процессов-сыновей (1 ≤ count ≤ 100).

На стандартном потоке ввода подается последовательность 32-битных знаковых целых чисел в текстовом виде. Последовательность завершается признаком конца ввода.

Родитель создает count сыновей, которые нумеруются от 0 до count-1. Сыновья работают последовательно. В свою очередь каждый сын считывает одно число из входной последовательности, выводит на стандартный поток вывода свой номер и считанное число. Затем очередь работы передается сыну с номером, равным остатку (математическому) от деления считанного числа на количество процессов.

После создания всех процессов родитель передает очередь чтения процессу с номером 0. Затем родитель дожидается завершения работы сыновей и сам завершает работу.

Для синхронизации процессов используйте семафоры SysV IPC.

Идентификатор массива семафоров выбирайте самостоятельно. В конце работы программы все созданные объекты IPC должны быть уничтожены.

Стандартные потоки ввода и вывода являются каналами.

Ниже приведен пример работы при числе процессов 3.

Examples
Input
1 2 4 3 5 6 9 2
Output
0 1
1 2
2 4
1 3
0 5
2 6
0 9
0 2
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