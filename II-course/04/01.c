#include <stdio.h>

int main(void) {
    int n, w;
    if (scanf("%u %u", &n, &w) != 2) {
        return 1;
    }
    unsigned int number = 0, mask = (0x1 << n) - 1, overflowed = mask + 1, max = mask >> 1;
    do {
        int n = number > max ? (-1)*(overflowed - number) : number;
        printf("|%*u|%*x|%*d|\n", w, number, w, number, w, n);
        number++;
    } while((number & mask) != 0);
    return 0;
}