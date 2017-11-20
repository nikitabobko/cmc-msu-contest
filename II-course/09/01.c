#include <stdio.h>
#include <string.h>

enum 
{
    TABLE_ITEMS_COUNT = 1024,
    TABLE_SIZE = 4096,
    BUF_SIZE = 32,
    RIGHT_SHIFT_TO_GET_ROOT_TABLE_OFFSET = 10 + 12,
};

int get_range(FILE *f, unsigned *a, unsigned *b) {
    char buf[BUF_SIZE];
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
    char root_table[TABLE_ITEMS_COUNT] = {};
    unsigned a, b, count = 0;
    while (get_range(stdin, &a, &b)) {
        for (unsigned i = a; i < b; i += TABLE_SIZE) {
            count += (root_table[i >> RIGHT_SHIFT_TO_GET_ROOT_TABLE_OFFSET] == 0);
            root_table[i >> RIGHT_SHIFT_TO_GET_ROOT_TABLE_OFFSET] = 1;
        }
    }
    printf("%u\n", (count > 0 ? count + 1 : count) * TABLE_SIZE);
    return 0;
}
