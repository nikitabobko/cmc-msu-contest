#include <stdlib.h>
#include <math.h>
#include "rand.h"

int rand_range(int low, int high) {
    return floor(rand()/(RAND_MAX + 1.) * (high - low) + low);
}