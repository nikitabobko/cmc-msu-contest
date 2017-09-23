/*
Submit a solution for up01-5-c/algorithms/sort-double-2

Full score:	100
Run penalty:	10
Time limit:	500 ms
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/09/05 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up01-5: c/algorithms/sort-double-2

На стандартном потоке ввода вводится последовательность пар из вещественного (double) и целого (int) числа. Максимальное число пар не превышает 16384.

На стандартный поток вывода напечатайте введенные пары, отсортированные в порядке неубывания первого числа в паре. Для равных значений первого числа должен сохраниться исходный (до сортировки) порядок пар.

Для печати вещественных чисел используйте форматное преобразование %.10g

Examples

Input

6.5 10
1.2 5
5.4 4
1.2 1
Output

1.2 5
1.2 1
5.4 4
6.5 10
*/

#include <stdio.h>
#include <stdlib.h>

enum 
{
    MAX_NUM_OF_PAIRS = 16384
};

typedef struct Pair
{
    double first;
    int second;
    int index;
} Pair;

int 
comparator(const void *first, const void *second) {
    double a = ((Pair *) first)->first, b = ((Pair *) second)->first;
    if (a > b) {
        return 1;
    }
    if (b > a) {
        return -1;
    }
    return ((Pair *) first)->index - ((Pair *) second)->index;
}

int 
main(int argc, char const *argv[])
{
    Pair arr[MAX_NUM_OF_PAIRS];
    int arr_length = 0;
    while (scanf("%lf %d", &(arr[arr_length].first), &(arr[arr_length].second)) != EOF) {
        arr[arr_length].index = arr_length;
        arr_length++;
    }
    qsort(arr, arr_length, sizeof(*arr), &comparator);
    for (int i = 0; i < arr_length; i++) {
        printf("%.10g %d\n", arr[i].first, arr[i].second);
    }
    return 0;
}