#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int length(unsigned char *str) {
    if (str == NULL) {
        return 0;
    }
    int len = 0;
    while (*str != '\0') {
        str += code_point_to_bytes(*str);
        len++;
    }
    return len;
}

unsigned char *read_word(void) {
    int c;
    do {
        c = fgetc(stdin);
    } while (c != EOF && c <= MAX_SPACE_CODE_POINT);
    if (c == EOF) {
        return NULL;
    }
    unsigned char *str = NULL;
    int size = 0, pos = 0;
    do {
        if (str == NULL) {
            size = DEFAULT_CAPACITY;
            str = malloc(size * sizeof(*str));
            if (str == NULL) {
                return NULL;
            }
        }
        if (pos + 1 >= size) {
            size *= 2;
            char *ptr = realloc(str, size * sizeof(*str));
            if (ptr == NULL) {
                free(str);
                return NULL;
            }
            str = ptr;
        }
        str[pos++] = (c == '\0' ? MAX_SPACE_CODE_POINT : c);
    } while ((c = fgetc(stdin)) != EOF && c > MAX_SPACE_CODE_POINT);
    str[pos] = '\0';
    return str;
}

int main(void) {
    clock_t begin = clock();

    int max_len = 0, len;
    char *max_len_str = NULL, *str;
    while ((str = read_word())) {
        if ((len = length(str)) > max_len) {
            max_len = len;
            free(max_len_str);
            max_len_str = str;
        } else {
            free(str);
        }
    }

    printf("%d\n", max_len);
    if (max_len > 0) {
        printf("%s\n", max_len_str);
    }

    free(max_len_str);

    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}