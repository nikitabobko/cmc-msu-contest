/*
Submit a solution for up04-1-c/ints/twos-compl-table-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/26 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up04-1: c/ints/twos-compl-table-1

На стандартном потоке ввода вводятся два целых числа n (1 ≤ n ≤ 17) и w (1 ≤ w ≤ 1000). Число n задает степень 2, а число w — ширину поля вывода.

На стандартный поток вывода выведите таблицу из трех столбцов, показывающую соответствие между целыми беззнаковыми n-битными числами, шестнадцатеричными числами и целыми знаковыми n-битными числами в дополнительном коде как показано на примере n = 4. Параметр w задает ширину столбца вывода в символах. Для разделения столбцов должен использоваться символ '|' (вертикальная черта). В конце каждой строки выводится символ \n.

Вывод программы будет сравниваться с эталонным побайтово. Не должно быть лишних пробелов или пустых строк.

Examples

Input
4 6
Output
|     0|     0|     0|
|     1|     1|     1|
|     2|     2|     2|
|     3|     3|     3|
|     4|     4|     4|
|     5|     5|     5|
|     6|     6|     6|
|     7|     7|     7|
|     8|     8|    -8|
|     9|     9|    -7|
|    10|     a|    -6|
|    11|     b|    -5|
|    12|     c|    -4|
|    13|     d|    -3|
|    14|     e|    -2|
|    15|     f|    -1|
*/
#include <stdio.h>

int main(void) {
    int n, w;
    if (scanf("%u %u", &n, &w) != 2) {
        return 1;
    }
    unsigned int number = 0;
    unsigned int mask = (0x1 << n) - 1;
    unsigned int overflowed = mask + 1;
    unsigned int max = mask >> 1;
    do {
        int n = number > max ? (-1)*(overflowed - number) : number;
        printf("|%*u|%*x|%*d|\n", w, number, w, number, w, n);
        number++;
    } while((number & mask) != 0);
    return 0;
}