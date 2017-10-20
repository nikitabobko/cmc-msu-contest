#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum 
{
    DEFAULT_CAPACITY = 32,
    MAX_SPACE_CODE_POINT = 0x20
};

int code_point_to_bytes(unsigned char c) {
    if (c >> 7 == 0) {
        return 1;
    } else if (c >> 5 == 0x6) {
        return 2;
    } else if (c >> 4 == 0xe) {
        return 3;
    } else if (c >> 3 == 0x1e) {
        return 4;
    } else if (c >> 2 == 0x3e) {
        return 5;
    } else if (c >> 1 == 0x7e) {
        return 6;
    }
    exit(1);
}

unsigned char *read_word(unsigned char *buf, int *size, int *len) {
    *len = 0;
    int c;
    do {
        c = getc(stdin);
    } while (c != EOF && c <= MAX_SPACE_CODE_POINT);
    if (c == EOF) {
        return NULL;
    }
    int pos = 0, size_local = *size, wait = 0, len_local = 0;
    do {
        if (buf == NULL) {
            size_local = DEFAULT_CAPACITY;
            buf = malloc(size_local * sizeof(*buf));
            if (buf == NULL) {
                return NULL;
            }
        }
        if (pos + 1 >= size_local) {
            size_local *= 2;
            char *ptr = realloc(buf, size_local * sizeof(*buf));
            if (ptr == NULL) {
                free(buf);
                return NULL;
            }
            buf = ptr;
        }
        buf[pos++] = (c == '\0' ? MAX_SPACE_CODE_POINT : c);

        if (!wait) {
            len_local++;
            wait += code_point_to_bytes(buf[pos - 1]) - 1;
        } else {
            wait--;
        }
    } while ((c = getc(stdin)) != EOF && c > MAX_SPACE_CODE_POINT);
    buf[pos] = '\0';
    *size = size_local;
    *len = len_local;
    return buf;
}

int main(void) {
    int max_len = 0, len, buf_size = 0;
    char *max_len_str = NULL, *str = NULL;
    while ((str = read_word(str, &buf_size, &len))) {
        if (len > max_len) {
            max_len = len;
            free(max_len_str);
            max_len_str = strdup(str);
        }
    }

    printf("%d\n", max_len);
    if (max_len > 0) {
        printf("%s\n", max_len_str);
    }

    free(max_len_str);
    free(str);

    return 0;
}