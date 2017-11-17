#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>

enum 
{
    PAGE_SIZE = 4096,
    BUF_SIZE = 32,
    RIGHT_SHIFT_TO_GET_ROOT_PAGE_OFFSET = 22
};

int get_range(FILE *f, unsigned *a, unsigned *b) {
    static char buf[BUF_SIZE];
    if (!fgets(buf, sizeof(buf), f)) {
        return 0;
    }
    if (buf[strlen(buf) - 1] != '\n') {
        int c;
        do {
            c = fgetc(f);
        } while (c != '\n' && c != EOF);
    }
    sscanf(buf, "%x-%x", a, b);
    return 1;
}

int main(int argc, char const *argv[]) {
    char root_page[PAGE_SIZE] = {};
    unsigned a, b, count = 0;
    while (get_range(stdin, &a, &b)) {
        a >>= RIGHT_SHIFT_TO_GET_ROOT_PAGE_OFFSET;
        b >>= RIGHT_SHIFT_TO_GET_ROOT_PAGE_OFFSET;
        for (unsigned i = a; i <= b; i++) {
            count += (root_page[i] == 0);
            root_page[i] = 1;
        }
    }
    printf("%u\n", (count > 0 ? count + 1 : count) * PAGE_SIZE);
    return 0;
}