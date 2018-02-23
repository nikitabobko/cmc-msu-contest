/*
Submit a solution for ht01-3-c/algorithms/qsort-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/22 16:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ht01-3: c/algorithms/qsort-1

Напишите функцию:

void process(size_t count, int *data, int *order);
В нужные места в прототипе этой функции добавьте ключевое слово const.

Функция process получает на вход count целых чисел, размещающихся в массиве data. Параметр order указывает на область памяти (массив), достаточную для размещения count значений типа int.

Функция process должна сформировать выходной массив order таким образом, что элемент order[i] должен хранить индекс j такой, что если бы массив data был бы отсортирован по возрастанию в массиве data_sorted, то data_sorted[i] == data[j].

Если два элемента равны, то должно сохраняться их относительное положение. То есть индексы j равных элементов в массиве order должны идти в порядке возрастания.

Например, если входной массив содержит числа:

{5, 4, 3, 2, 1}
Массив order должен после работы функции быть равен:

{4, 3, 2, 1, 0}
А если входной массив содержит числа:

{5, 3, 3, 2, 1}
То результат работы должен быть равен:

{4, 3, 1, 2, 0}
Для сортировки используйте функцию qsort_r. Можете предполагать, что ваша программа будет компилироваться с необходимыми для ее использования опциями препроцессора.

Не используйте глобальные переменные и временные массивы.

Вы должны сдать только текст функции (и необходимые для ее компиляции директивы #include). Текст не должен содержать функцию main.
*/

#include <stdlib.h>

int 
compar(const void *first, const void *second, void *thunk) 
{
    int first_index = *((int *) first);
    int second_index = *((int *) second);
    const int *data = (const int *) thunk;
    if (data[first_index] == data[second_index]) {
        return first_index - second_index;
    }
    if (data[first_index] > data[second_index]) {
        return 1;
    }
    return -1;
}

void 
process(size_t count, const int *data, int *order) 
{
    if (count <= 0 || data == NULL || order == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        order[i] = i;
    }
    qsort_r(order, count, sizeof(*order), &compar, (void *) data);
}