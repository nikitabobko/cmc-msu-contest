#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++) {
        char *endptr = NULL;
        unsigned long int val = strtoul(argv[i], &endptr, 10);
        if ((endptr != NULL && *endptr != '\0') || (val == ULONG_MAX && errno == ERANGE)) {
            printf("-1\n");
            continue;
        }
        printf("%lf\n", ceil(log2((double) val + 1)));
    }
    return 0;
}