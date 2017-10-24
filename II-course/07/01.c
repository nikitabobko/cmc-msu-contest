/*
Submit a solution for up07-1-c/random/random-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/24 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up07-1: c/random/random-1

Программе в аргументах командной строки задаются четыре целых 32-битных числа:

count (count >= 0, count < 100000) – число чисел для генерации;
low (low >= -32768);
high (0 <= high <= 32768, low < high);
seed (seed > 0).
На стандартный поток вывода напечатайте count псевдослучайных чисел в интервале [low, high), полученных с помощью функции rand() с начальным значением генератора, задаваемым seed.

Для получения случайных чисел из значения, возвращаемого rand() сначала получите вещественное число в полуинтервале [0;1), а затем из него получите требуемое целое число.

Пример выполнения программы:

./solution 5 0 10 555
8
2
9
1
8
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    if (argc != 5) {
        return 0;
    }
    unsigned count = strtol(argv[1], NULL, 10);
    int low = strtol(argv[2], NULL, 10);
    unsigned high = strtol(argv[3], NULL, 10);
    unsigned seed = strtol(argv[4], NULL, 10);
    srand(seed);

    for (int i = 0; i < count; i++) {
        printf("%d\n", (int) floor(rand()/(RAND_MAX + 1.) * (high - low) + low));
    }
    return 0;
}