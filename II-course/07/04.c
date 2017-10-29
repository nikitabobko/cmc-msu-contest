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