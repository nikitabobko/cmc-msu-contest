#include <stdlib.h>
#include <stdio.h>

typedef struct RandomGenerator
{
    struct RandomGenerator *ops;
    int (*next)(struct RandomGenerator *rr);
    void (*destroy)(struct RandomGenerator *rr);
    int previous;
} RandomGenerator;

int next_fun(RandomGenerator *rr) {
    int n = (1103515245*rr->previous + 12345) % 0x80000000;
    return rr->previous = n;
}

void destory_fun(RandomGenerator *rr) {
    free(rr);
}

RandomGenerator *random_create(int seed) {
    RandomGenerator *arr = calloc(1, sizeof(*arr));
    arr[0].ops = arr;
    arr[0].next = &next_fun;
    arr[0].destroy = &destory_fun;
    arr[0].previous = seed;
    return arr;
}