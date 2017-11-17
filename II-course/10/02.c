/*
Submit a solution for up10-2-unix/pipe/pipe-c

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    1 s
Memory limit:   64M
Open date:  2017/11/14 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up10-2: unix/pipe/pipe-c

Процесс-родитель создает процесса-сына, а тот в свою очеред процесса-внука. Процесс-родитель и процесс-внук должны быть соединены анонимным каналом в направлении от родителя к внуку.

Процесс-родитель считывает 32-битные знаковые целые числа, подаваемые на стандартном потоке ввода в текстовом виде. Последовательность заканчивается признаком конца файла. Процесс-родитель передает считанные числа в канал в бинарном виде.

Процесс-внук считывает числа из канала и вычисляет их сумму. После чтения всех чисел процесс-внук выводит на стандартный поток вывода их сумму и завершает работу.

Процесс-родитель должен дождаться завершения всех порожденных им процессов и завершиться сам с кодом завершения 0.

Ошибка "Synchronization error" означает, что родитель завершился раньше какого-либо из порожденных им процессов.
*/
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    int fd[2];
    pipe(fd);
    if (!fork()) {
        close(fd[1]);
        if (!fork()) {
            int num;
            long long sum = 0;
            while (read(fd[0], &num, sizeof(num)) > 0) {
                sum += num;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            exit(0);
        }
        close(fd[0]);

        wait(NULL);
        exit(0);
    }
    close(fd[0]);
    int num;
    while (scanf("%d", &num) == 1) {
        write(fd[1], &num, sizeof(num));
    }
    close(fd[1]);
    wait(NULL);
    return 0;
}