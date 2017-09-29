#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

// calculates ceil(log2(x+1)) not using doubles
int ceil_log2_plus_one(uint64_t val) {
    val++;
    int count = 0;
    int is_power_of_two = 1;
    while (val != 1) {
        count++;
        is_power_of_two &= ~val & 0x1;
        val >>= 1;
    }
    return count + !is_power_of_two;
}

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++) {
        char *endptr = NULL;
        errno = 0;
        uint64_t val = strtoul(argv[i], &endptr, 10);
        if ((endptr != NULL && *endptr != '\0') || errno == ERANGE || val > UINT32_MAX) {
            printf("-1\n");
            continue;
        }
        printf("%d\n", ceil_log2_plus_one(val));
    }
    return 0;
}