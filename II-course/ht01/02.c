/*
Submit a solution for ht01-2-c/arguments/ln2-args-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/22 16:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ht01-2: c/arguments/ln2-args-1

Программе передаются аргументы командной строки.

Некоторые из этих аргументов являются 32-битными беззнаковыми целыми числами, другие аргументы — не являются.

Аргумент является 32-битным беззнаковым целым числом, если, пропустив начальные пробельные символы, он состоит только из десятичных цифр, перед которыми могут находиться знаки "плюс" или "минус", и его значение попадает в диапазон представимых чисел. Пробельные символы в "хвосте" числа не допускаются.

Для беззнаковых чисел разрешен знак минус, так как стандартная функция strtoul поддерживает знак минус перед числом. Однако, при использовании strtoul на 64-битной платформе знак минус перед числом выводит число из диапазона представимых чисел. Поэтому поведение программы на 32-битной и 64-битной платформе будет отличаться из-за разного размера типа long и поведения функции strtoul.

Для каждого аргумента, являющегося 32-битным беззнаковым целым числом, на стандартный поток вывода на отдельной строке напечатайте значение, равное ceil(log2((double) val + 1)), где val — значение аргумента. Для прочих аргументов командной строки напечатайте на отдельной строке −1. Порядок вывода должен совпадать с порядком следования аргументов.

Для переносимой работы с целыми типами фиксированной битности используйте типы из заголовочного файла stdint.h

Программа должна работать корректно для чисел, не начинающихся со знака '-', и на 32-битных, и на 64-битных системах (см. также замечание выше). В тестах будут отсутствовать отрицательные числа.

Не используйте вещественную арифметику.

Для преобразования строки в число используйте функцию strtoul стандартной библиотеки C.

Не забывайте выводить символ конца строки в конце вывода.
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

int bitcount(uint64_t val) {
    int count = 0;
    while (val != 0) {
        count++;
        val >>= 1;
    }
    return count;
}

int main(int argc, char const *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *endptr = NULL;
        errno = 0;
        uint64_t val = strtoul(argv[i], &endptr, 10);
        if ((endptr != NULL && *endptr != '\0') || errno == ERANGE || val > UINT32_MAX) {
            printf("-1\n");
        } else {
            printf("%d\n", bitcount(val));
        }
    }
    return 0;
}