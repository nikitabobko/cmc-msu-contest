/*
Submit a solution for up01-2-c/floats/uint-fit-1

Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/09/05 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up01-2: c/floats/uint-fit-1

На стандартном потоке ввода задается последовательность 32-битных беззнаковых целых чисел. Последовательность заканчивается с концом файла.

Для каждого введенного числа на отдельной строке напечатайте 1, если это число может быть точно представлено в виде значения типа `float`, и 0 в противном случае.

В решении не используйте операции с вещественными числами.
*/

#include <stdio.h>

// returns 1 if int number can be presented as float; otherwise returns 0
char 
can_be_presented_as_float(unsigned a) 
{
    if (a == 0) {
        return 1;
    }
    while ((a & 0x1) != 1) {
        a >>= 1;
    }
    int i = 0;
    while (a != 1) {
        a >>= 1;
        i++;
    }
    return i <= 23;
}

int 
main(void)
{
    unsigned a;
    while (scanf("%u", &a) == 1) {
        printf("%d\n", can_be_presented_as_float(a));
    }
    return 0;
}