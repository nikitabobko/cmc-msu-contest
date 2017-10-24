#include "rand.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if (argc != 5) {
        return 0;
    }
    unsigned count = strtol(argv[1], NULL, 10);
    int low = strtol(argv[2], NULL, 10);
    unsigned high = strtol(argv[3], NULL, 10);
    unsigned seed = strtol(argv[4], NULL, 10);
    srand(seed);

    for (int i = 0; i < count; i++) {
        printf("%d\n", rand_range(low, high));
    }
    return 0;
}