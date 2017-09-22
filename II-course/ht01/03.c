#include <stdlib.h>

int compar(const void *first, const void *second, void *thunk) 
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