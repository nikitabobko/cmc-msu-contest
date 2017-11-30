/*
Submit a solution for up11-2-unix/signal/signal-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    1 s
Memory limit:   64M
Open date:  2017/11/21 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up11-2: unix/signal/signal-1

Программа должна напечатать на стандартный поток вывода свой pid.

Затем программе на стандартном потоке ввода подаются целые числа. В зависимости от режима работы для каждого введенного числа на стандартный поток вывода программа выводит либо число, взятое с обратным знаком, либо квадрат числа.

При получении сигнала SIGUSR1 программа переключается в режим взятия обратного знака числа. При получении сигнала SIGUSR2 программа переключается в режим возвведения числа в квадрат.

Стандартный ввод и стандартный вывод программы будут перенаправлены. Не забывайте сбрасывать буфер вывода. С другой стороны каналов находится другой процесс, который начнет свою работу как только получит pid процесса.

По умолчанию программа работает в режиме взятия обратного знака числа.
*/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef enum Mode {
    Mode1, Mode2
} Mode;

volatile Mode mode = Mode1;

void sighandler(int sig) {
    if (sig == SIGUSR1) {
        mode = Mode1;
    } else {
        mode = Mode2;
    }
    signal(SIGUSR1, &sighandler);
    signal(SIGUSR2, &sighandler);
}

int main(int argc, char const *argv[]) {
    signal(SIGUSR1, &sighandler);
    signal(SIGUSR2, &sighandler);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int num;
    while (scanf("%d", &num) == 1) {
        if (mode == Mode1) {
            printf("%d\n", -num);
        } else if (mode == Mode2) {
            printf("%d\n", num * num);
        }
        fflush(stdout);
    }
    return 0;
}