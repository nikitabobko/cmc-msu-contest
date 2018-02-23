/*
Submit a solution for up07-3-c/random/random-with-make-1

Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/10/24 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up07-3: c/random/random-with-make-1

Модифицируйте решение задачи c/random/random-1 следующим образом: генерацию случайных чисел в диапазоне [low, high) реализуйте в виде отдельной функции

int rand_range(int low, int high);
Функция должна находиться в отдельном файле rand.c и заголовочном файле rand.h. Реализуйте файл main.c, который будет решать задачу mz07-1. Напишите Makefile для сборки проекта. Исполняемый файл должен называться genrand, а каталог проекта – genrand. В сдаваемом каталоге не должны находиться объектные и исполняемые файлы, а также файлы с суффиксом .bak и ~.

Файлы Makefile и rand.h должны следовать требованиям, описанным в методическом пособии. Обязательно должны использоваться переменные CC и CFLAGS!

В тестирующую систему должен сдаваться tar.gz архив. Если каталог называется genrand, то архив genrand.tar.gz можно получить следующей командой из каталога, в котором находится genrand:

tar cfz genrand.tar.gz genrand
Напоминание: rand.h должен быть защищен от повторного включения. rand.c должен включать rand.h.
*/
#include "rand.h"
#include <stdio.h>
#include <stdlib.h>

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
        printf("%d\n", rand_range(low, high));
    }
    return 0;
}