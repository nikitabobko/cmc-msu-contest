/*
Submit a solution for ht03-3-unix/dl/dl-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/31 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem ht03-3: unix/dl/dl-1

Программе в аргументах командной строки передается имя функции. Функция находится в стандартной математической библиотеке Си, принимает один аргумент типа double и возвращает результат типа double.

На стандартном потоке ввода вводится последовательность вещественных чисел. На стандартном потоке вывода напечатайте результат вычисления функции от введенных аргументов. Для вывода используйте форматное преобразование %.10g функции printf.

Математическая библиотека Си находится в файле /lib/libm.so.6.

Пример для функции sin.

Examples

Input
0.5 0.8
Output
0.4794255386
0.7173560909
*/
#include <stdio.h>
#include <dlfcn.h>

static const char *PATH_TO_LIB = "/lib/libm.so.6";

int main(int argc, const char **argv) {
    if (argc < 2) {
        return 0;
    }

    void *handle = dlopen(PATH_TO_LIB, RTLD_LAZY);
    if (!handle) {
        return 0;
    }

    double (*fun)(double) = dlsym(handle, argv[1]);
    if (!fun) {
        return 0;
    }

    double num;
    while (scanf("%lf", &num) == 1) {
        printf("%.10g\n", fun(num));
    }

    dlclose(handle);
    return 0;
}