/*
Submit a solution for up11-1-unix/signal/signal-8

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    1 s
Memory limit:   64M
Open date:  2017/11/21 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up11-1: unix/signal/signal-8

Программа должна напечатать на стандартный поток вывода свой PID, после чего перейти в режим ожидания сигналов. Программа должна обрабатывать сигнал SIGINT. На каждый приход сигнала программа должна напечатать номер поступления сигнала (0, 1, 2) — каждый раз на отдельной строке. На пятый приход сигнала программа не должна ничего выводить, а просто завершиться с кодом завершения 0.

Таким образом, программа всегда должна выводить:

СВОЙ-PID
0
1
2
3
Стандартный ввод и стандартный вывод программы будут перенаправлены. Не забывайте выводить разделитель и сбрасывать буфер вывода. С другой стороны каналов находится другой процесс, который начнет свою работу как только получит pid процесса.

Не используйте активное ожидание сигнала.

Вывод на стандартный поток вывода разместите в обработчике сигнала.

Вариант семантики сигналов (SysV или BSD), используемый функцией signal, неизвестен.
*/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void sighandler(int sig) {
    static int i = 0;
    if (i == 4) {
        exit(0);
    }
    printf("%d\n", i++);
    fflush(stdout);
    signal(SIGINT, &sighandler);
}

int main(int argc, char const *argv[]) {
    signal(SIGINT, &sighandler);
    printf("%d\n", getpid());
    fflush(stdout);
    pause();
    pause();
    pause();
    pause();
    pause();
    return 0;
}