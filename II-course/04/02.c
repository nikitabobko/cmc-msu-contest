#include <stdio.h>

int main(int argc, char *argv[]) {
    const char pattern[] = "rwxrwxrwx";
    for (int i = 1; i < argc; i++) {
        unsigned num;
        sscanf(argv[i], "%o", &num);
        char str[sizeof(pattern)] = {};
        for (int k = 0; k < sizeof(pattern) - 1; k++) {
            str[k] = (num >> (sizeof(pattern) - 2 - k)) & 0x1 ? pattern[k] : '-';
        }
        printf("%s\n", str);
    }
    return 0;
}