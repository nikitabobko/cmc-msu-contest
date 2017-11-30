#include<stdio.h>
#include<string.h>

enum
{
    BUF_SIZE = 2048
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
    unsigned write_count = 0, exec_count = 0;
    char buf[BUF_SIZE];
    while(read_line(buf, sizeof(buf), stdin)) {
        unsigned a, b;
        char buf2[BUF_SIZE];
        sscanf(buf, "%x-%x %s", &a, &b, buf2);
        if (buf2[1] == 'w') {
            write_count += b - a;
        }
        if (buf2[2] == 'x') {
            exec_count += b - a;
        }
    }
    printf("%u\n%u\n", write_count, exec_count);
}