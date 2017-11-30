/*
Submit a solution for ht03-5-unix/mmap/ping-pong-b

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/31 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem ht03-5: unix/mmap/ping-pong-b

В аргументах командной строки задается целое неотрицательное число N. Родитель должен создать два процесса. Оба процесса работают поочередно. Сначала первый процесс выводит на стандартный поток вывода свой номер (1) и число 1, затем второй процесс выводит на стандартный поток вывода свой номер (2) и число, на 1 больше полученного от первого процесса (2). Затем первый процесс выводит на стандартный поток вывода свой номер (1) и число, на единицу больше числа, полученного от второго процесса и т. д.

Последним выводится номер соответствующего процесса и число N. После этого процессы должны завершить свою работы. Родитель дожидается их завершения и завершается сам.

Таким образом, если в командной строке задан параметр 4, на стандартном потоке вывода должно быть выведено

1 1
2 2
1 3
2 4
Для передачи данных используйте разделяемую память, созданную с помощью mmap. Для уменьшения нагрузки на процессор используйте sched_yield. Учитывайте особенности буферизации потоков вывода.

Не используйте примитивы синхронизации, предоставляемые операционной системой. Не используйте атомарные операции (atomic), в том числе расширения gcc. Можете использовать синхронизацию с помощью инструкции процессора XCHG, используемой с помощью встраиваемого ассемблера.

Внимание! Способы синхронизации, использующие активное ожидание и неатомарные операции проверки-изменения содержимого ячейки памяти не являются ни корректными, ни переносимыми. В данной задаче они даны только для ознакомительных целей.
*/
#include <sys/mman.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

enum
{
    SHARED_MEM_SIZE = 3 * sizeof(int),
    FIRST_LOCKED_CHILD = 2,
    NUMERICAL_SYSTEM_BASE = 10,
    LOCK_ONE_INDEX = 0,
    LOCK_TWO_INDEX = 1,
    NUM_INDEX = 2,
};

void play_ping_pong(int n_times, int child, volatile int *own_lock_mem, 
        volatile int *other_child_lock_mem, volatile int *num_mem) {
    if (child == FIRST_LOCKED_CHILD) {
        // Spin
        while(*own_lock_mem) {
            sched_yield();
        }
    }
    while (*num_mem <= n_times) {
        printf("%d %d\n", child, (*num_mem)++);
        fflush(stdout);

        // Lock itself
        *own_lock_mem = 1;
        // Unlock other child
        *other_child_lock_mem = 0;
        // Spin
        while(*own_lock_mem) {
            sched_yield();
        }
    }
    // Unlock other child
    *other_child_lock_mem = 0;
    exit(0);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 0;
    }
    // Shared memory
    volatile int *mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        return 0;
    }
    mem[LOCK_ONE_INDEX] = 0;
    mem[LOCK_TWO_INDEX] = 1;
    mem[NUM_INDEX] = 1;

    errno = 0;
    char *endptr;
    long num = strtol(argv[1], &endptr, NUMERICAL_SYSTEM_BASE);
    if (errno || *endptr != '\0' || num > INT_MAX || num < INT_MIN) {
        return 0;
    }

    pid_t pid = fork();
    if (!pid) {
        play_ping_pong(num, 1, mem + LOCK_ONE_INDEX, mem + LOCK_TWO_INDEX, mem + NUM_INDEX);
    } else if (pid == -1) {
        return 0;
    }
    pid = fork();
    if (!pid) {
        play_ping_pong(num, 2, mem + LOCK_TWO_INDEX, mem + LOCK_ONE_INDEX, mem + NUM_INDEX);
    } else if (pid == -1) {
        return 0;
    }
    wait(NULL);
    wait(NULL);

    return 0;
}