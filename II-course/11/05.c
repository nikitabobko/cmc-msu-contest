/*
Submit a solution for up11-5-unix/signal/signal-4

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    2 s
Memory limit:   64M
Open date:  2017/11/21 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up11-5: unix/signal/signal-4

Программе на стандартном потоке ввода задаются два целых числа low и high. Программа должна вывести на стандартный поток вывода свой PID, после вывода программа должна обрабатывать получение сигналов SIGINT и SIGTERM.

Программа должна вычислять простые числа в интервале [low,high). Если программа получила сигнал SIGINT, программа должна вывести на стандартный поток вывода последнее на текущей момент найденное простое число. При получении SIGINT в четвертый раз, программа должна закончить работу с кодом завершения 0. Таким образом, может быть выведено не более трех простых чисел.

При получении сигнала SIGTERM программа должна завершиться с кодом 0.

Если программа закончила вычисления, программа должна вывести на стандартный поток вывода число -1 и завершиться с кодом 0.

Если на момент прихода сигнала ни одно простое число найдено не было, выводите 0.

Учтите, что стандартный поток вывода программы будет перенаправлен в pipe.
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

enum 
{
    SIG_INT_COUNT_TO_EXIT = 4
};

volatile int last_found_prime = 0;
int sig_int_count = 0;

void hndl(int sig) {
    if (sig == SIGTERM) {
        exit(0);
    } else { // sig == SIGINT
        sig_int_count++;
        if (sig_int_count == SIG_INT_COUNT_TO_EXIT) {
            exit(0);
        }
        printf("%d\n", last_found_prime);
        fflush(stdout);
        signal(SIGINT, &hndl);
    }
}

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i*i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char const *argv[]) {
    signal(SIGTERM, &hndl);
    signal(SIGINT, &hndl);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int low, high;
    scanf("%d %d", &low, &high);

    for (int i = low; i < high; i++) {
        if (is_prime(i)) {
            last_found_prime = i;
        }
    }

    printf("-1\n");
    fflush(stdout);

    return 0;
}