#include <stdint.h>

int32_t satsum(int32_t v1, int32_t v2) {
    int32_t value = v1 + v2;
    if (v1 < 0 && v2 < 0 && value >= 0) {
        return INT32_MIN;
    }
    if (v1 > 0 && v2 > 0 && value <= 0) {
        return INT32_MAX;
    }
    return value;
}
