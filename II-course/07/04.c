#include <stdlib.h>
#include <stdio.h>

enum 
{
    MASK = 0x7fffffff,
    INCREMENT = 12345,
    MULTIPLIER = 1103515245 
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
    RandomOperations *ops;
    int previous;
};

static RandomOperations ops_global;

int next_fun(RandomGenerator *rr) {
    return rr->previous = (MULTIPLIER*rr->previous + INCREMENT) & MASK;
}

void destory_fun(RandomGenerator *rr) {
    free(rr);
}

RandomGenerator *random_create(int seed) {
    RandomGenerator *ptr = calloc(1, sizeof(*ptr));
    if (ptr == NULL) {
        free(ptr);
        return NULL;
    }
    ops_global.next = &next_fun;
    ops_global.destroy = &destory_fun;
    ptr->ops = &ops_global;
    ptr->previous = seed;
    return ptr;
}