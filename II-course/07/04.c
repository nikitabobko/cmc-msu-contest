/*
Submit a solution for up07-4-c/random/random-ops-3

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/24 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up07-4: c/random/random-ops-3

Определите структуру RandomOperations с полями destroy и next — указателями на функции.

Определите структуру RandomGenerator с необходимыми полями.

typedef struct RandomGenerator
{
//…
} RandomGenerator;
напишите реализацию функции

RandomGenerator *random_create(int seed);
напишите реализации недостающих функций таким образом, чтобы фрагмент

RandomGenerator *rr = random_create(1234);
for (int j = 0; j < 100; ++j) {
  printf("%d\n", rr->ops->next(rr));
}
rr->ops->destroy(rr);
выводил на печать 100 псевдослучайных чисел, полученных с помощью линейного конгруэнтного генератора (Linear congruential generator) с начальным значением 1234, мультипликатором (a) 1103515245, инкрементом (c) 12345 и модулем 2^31. Затем вызов destroy освобождает память, выделенную функцией random_create.

Функция next должна возвращать следующий элемент псевдослучайной последовательности отнсительно текущего состояния.

Сдаваемый на проверку фрагмент программы должен содержать определение структур RandomOperations, RandomGenerator определение функции random_create и других функций, кроме main.

Указание. Структуру с указателями на функции можно сделать глобальной константой и использовать ее для всех создаваемых объектов.
*/
#include <stdlib.h>
#include <stdio.h>

enum 
{
    MOD = 0x80000000,
    INCREMENT = 12345,
    MULTIPLIER = 1103515245,
};

typedef struct RandomGenerator RandomGenerator;
typedef struct RandomOperations RandomOperations;

struct RandomOperations
{
    int (*next)(RandomGenerator *rr);
    void (*destroy)(RandomGenerator *rr);
};

struct RandomGenerator
{
    const RandomOperations *ops;
    int previous;
};

int next_fun(RandomGenerator *rr) {
    __builtin_mul_overflow(MULTIPLIER, rr->previous, &(rr->previous));
    __builtin_add_overflow(rr->previous, INCREMENT, &(rr->previous));
    return rr->previous = rr->previous % MOD;
}

void destory_fun(RandomGenerator *rr) {
    free(rr);
}

static const RandomOperations ops_global = {&next_fun, &destory_fun};

RandomGenerator *random_create(int seed) {
    RandomGenerator *ptr = calloc(1, sizeof(*ptr));
    if (ptr == NULL) {
        return NULL;
    }
    ptr->ops = &ops_global;
    ptr->previous = seed;
    return ptr;
}