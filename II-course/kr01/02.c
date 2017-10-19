/*
Submit a solution for ku01-2-kr01-2 (upsolving)

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   512M
Open date:  2017/10/17 16:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ku01-2: kr01-2 (upsolving)

В аргументах командной строки передаются списки 32-битных знаковых целых чисел. Каждый список представляет собой непустую последовательность чисел, разделенных знаком "запятая". Пробельные символы в списке отсутствуют. Входные данные корректны.

Для каждого аргумента командной строки — списка целых чисел на стандартный поток вывода выведите число целых чисел в интервале чисел, в котором находятся все эти числа. Например, для списка чисел 10,5,11,2,7 все числа в нем попадают в интервал [2;11], а всего в этом интервале находится 10 целых чисел.

Не забывайте выводить \n в конце.

Пример:

./solution 10,5,11,2,7 5
10
1

*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        long long min, max, num;
        int is_min_defined = 0, is_max_defined = 0;
        char *str = argv[i];
        do {
            num = strtol(str, &str, 10);
            if (!is_min_defined || num < min) {
                min = num;
                is_min_defined = 1;
            }
            if (!is_max_defined || num > max) {
                max = num;
                is_max_defined = 1;
            }
            if (*str == ',') {
                str++;
            }
        } while (*str != '\0');
        printf("%lld\n", max - min + 1);
    }
    return 0;
}