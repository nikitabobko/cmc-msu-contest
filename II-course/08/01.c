#include <stdio.h>
#include <string.h>

enum 
{
    BUF_SIZE = 32
};

int read_line(char *buf, int size, FILE *f) {
    if (!fgets(buf, size, f)) {
        return 0;
    }
    if (buf[strlen(buf) - 1] != '\n') {
        int c;
        do {
            c = fgetc(f);
        } while (c != '\n' && c != EOF);
    }
    return 1;
}

int main(void) {
    char buf[BUF_SIZE];
    unsigned sum = 0;
    while (read_line(buf, sizeof(buf), stdin)) {
        unsigned a, b;
        sscanf(buf, "%x-%x", &a, &b);
        sum += b - a;
    }
    printf("%u\n", sum);
    return 0;
}