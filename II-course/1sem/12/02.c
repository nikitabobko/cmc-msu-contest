/*
Submit a solution for up12-2-unix/ipc/sem-2
Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    2 s
Memory limit:   64M
Open date:  2017/12/05 10:30:00
Date penalty formula:   2017/12/31 0-50/14d
Next soft deadline: 2017/12/19 10:30:00
Problem up12-2: unix/ipc/sem-2
Программе в аргументах командной строки задаются:

Размер массива (count);
Ключ IPC (key);
Количество процессов (nproc);
Число итераций (iter_count);
Затравка ГПСЧ для каждого процесса (nproc аргументов).
Программа должна создать в разделяемой памяти массив из count элементов типа int и заполнить его значениями, считанными со стандартного потока ввода.

Далее программа должна создать nproc процессов, для каждого процесса проинициализировав ГПСЧ соответствующим значением из командной строки.

Все процессы должны выполнить iter_count итераций модификации массива по следующему алгоритму. На каждой итерации выбираются два случайных индекса в массиве (каждый индекс в диапазоне [0;count) ) и случайное значение в диапазоне [0; 10). Далее должна вызываться функция operation, которая выполняет следующие действия:

void
operation(int *data, int ind1, int ind2, int value)
{
    if (ind1 != ind2) {
        int tmp1 = data[ind1] - value;
        int tmp2 = data[ind2] + value;

        data[ind1] = tmp1;
        data[ind2] = tmp2;
    }
}
Считайте эту функцию написанной и доступной в вашей программе.

Родитель дожидается окончания всех сыновей и выводит на стандартный поток вывода значения массива.

Для синхронизации процессов используйте семафоры. Для пересылки значения используйте разделяемую память. Массив семафоров и разделяемая память должны уничтожаться в конце работы.

Должна быть обеспечена целостность массива.

Синхронизация должна позволять двум процессам одновременно модифицировать массив, если модифицируемые индексы массива не пересекаются.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

enum 
{
    MAX_VAL = 10,
    NUM_OF_ARGS = 5,
};

void kill_children(pid_t *pids, int nproc) {
    if (pids == NULL) {
        return;
    }
    for (int i = 0; i < nproc; i++) {
        kill(pids[i], SIGTERM);
    }
}

int main(int argc, char const *argv[]) {
    if (argc - 1 < NUM_OF_ARGS) {
        return 0;
    }
    int count = strtol(argv[1], NULL, 10);
    key_t key = strtol(argv[2], NULL, 10);
    int nproc = strtol(argv[3], NULL, 10);
    int iter_count = strtol(argv[4], NULL, 10);
    
    int memid = -1;
    int semid = -1;
    int *mem = NULL;
    int kill_children_flag = 0;
    pid_t *pids = malloc(nproc * sizeof(*pids));
    if (pids == NULL) {
        goto free_res;
    }
    if (argc - NUM_OF_ARGS != nproc) {
        return 1;
    }
    memid = shmget(key, count * sizeof(*mem), 0666 | IPC_CREAT);
    if (memid == -1) {
        goto free_res;
    }
    mem = shmat(memid, NULL, 0);
    for (int i = 0; i < count; i++) {
        scanf("%d", mem + i);
    }
    semid = semget(key, count, 0666 | IPC_CREAT);
    if (semid == -1) {
        goto free_res;
    }
    for (int i = 0; i < count; i++) {
        struct sembuf op = {i, 1, SEM_UNDO};
        if (semop(semid, &op, 1) < 0) {
            goto free_res;
        }
    }

    for (int i = 0; i < nproc; i++) {
        pids[i] = fork();
        if (!pids[i]) {
            srand(strtoul(argv[i + NUM_OF_ARGS], NULL, 10));
            for (int j = 0; j < iter_count; j++) {
                int ind1 = rand() / (RAND_MAX + 1.) * count; 
                int ind2 = rand() / (RAND_MAX + 1.) * count;
                int value = rand() / (RAND_MAX + 1.) * MAX_VAL;

                if (ind1 != ind2) {
                    struct sembuf down[] = {
                        {ind1, -1, SEM_UNDO}, 
                        {ind2, -1, SEM_UNDO},
                    };
                    if (semop(semid, down, sizeof(down)/sizeof(*down))) {
                        exit(1);
                    }
                    
                    operation(mem, ind1, ind2, value);
                    
                    struct sembuf up[] = {
                        {ind1, 1, SEM_UNDO}, 
                        {ind2, 1, SEM_UNDO},
                    };
                    if (semop(semid, up, sizeof(up)/sizeof(*up)) < 0) {
                        exit(1);
                    }
                }
            }
            exit(0);
        } else if (pids[i] < 0) {
            kill_children_flag = 1;
            goto free_res;
        }
    }

free_res:
    if (kill_children_flag) {
        kill_children(pids, nproc);
    }
    for (int i = 0; i < nproc; i++) {
        wait(NULL);
    }
    if (mem) {
        for (int i = 0; i < count; i++) {
            printf("%d%c", mem[i], i == count -1 ? '\n' : ' ');
        }
    }
    if (memid != -1) {
        shmctl(memid, IPC_RMID, NULL);
    }
    if (semid != -1) {
        semctl(semid, count, IPC_RMID);
    }
    free(pids);
    return 0;
}