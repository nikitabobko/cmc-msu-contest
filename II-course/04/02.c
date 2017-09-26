#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    char const str[] = "rwxrwxrwx";
    for (int i = 1; i < argc; i++) {
        unsigned num;
        sscanf(argv[i], "%o", &num);
        for (int k = 0; k < sizeof(str) - 1; k++) {
            printf("%c", (num >> (sizeof(str) - 2 - k)) & 0x1 ? str[k] : '-');
        }
        printf("\n");
    }
    return 0;
}