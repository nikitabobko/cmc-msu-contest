#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

int bitcount(uint64_t val) {
    int count = 0;
    while (val != 0) {
        count++;
        val >>= 1;
    }
    return count;
}

int main(int argc, char const *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *endptr = NULL;
        errno = 0;
        uint64_t val = strtoul(argv[i], &endptr, 10);
        if ((endptr != NULL && *endptr != '\0') || errno == ERANGE || val > UINT32_MAX) {
            printf("-1\n");
        } else {
            printf("%d\n", bitcount(val));
        }
    }
    return 0;
}