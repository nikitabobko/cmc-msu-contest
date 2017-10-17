#include <stdio.h>

int main(int argc, char const *argv[]) {
    unsigned num;
    while (scanf("%u", &num) == 1) {
        if (!num) {
            printf("00");
        }
        while (num) {
            unsigned octa = num & 0x7f;
            num >>= 7;
            if (num) {
                octa |= 0x80;
            }
            printf("%02x ", octa);
        }
        printf("\n");
    }
    return 0;
}