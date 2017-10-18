#include <stdio.h>
#include <stdlib.h>

enum 
{
    DEFAULT_CAPACITY = 16,
    MAX_SPACE_CODE_POINT = 0x20
};

int code_point_to_bytes(char *str) {
    if (*str >> 7 == 0) {
        return 1;
    } else if (*str >> 5 == 0x6) {
        return 2;
    } else if (*str >> 4 == 0xe) {
        return 3;
    } else if (*str >> 3 == 0x1e) {
        return 4;
    }
    return -1;
}

int read_word(unsigned char *str, unsigned char **word_start, unsigned char **word_end) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    while (*str <= MAX_SPACE_CODE_POINT) {
        str++;
    }
    *word_start = str;
    int len = 0;
    while (*str > MAX_SPACE_CODE_POINT) {
        str += code_point_to_bytes(str);
        len++;
    }
    *word_end = str;
    return len;
}

unsigned char *read_input() {
    unsigned char *str = NULL;
    int size = 0, pos = 0;
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        if (str == NULL) {
            size = DEFAULT_CAPACITY;
            str = malloc(size * sizeof(*str));
            if (str == NULL) {
                return NULL;
            }
        }
        if (pos + 1 >= size) {
            size *= 2;
            str = realloc(str, size * sizeof(*str));
            if (str == NULL) {
                return NULL;
            }
        }
        str[pos++] = (c == '\0' ? MAX_SPACE_CODE_POINT : c);
    }
    if (str != NULL) {
        str[pos] = '\0';
    }
    return str;
}

int main(int argc, unsigned char const *argv[]) {
    unsigned char *str = read_input();

    unsigned char *word_start, *word_end = str;
    unsigned char *max_word_start = NULL;
    unsigned char *max_word_end = NULL;
    int max_len = 0, len;
    while ((len = read_word(word_end, &word_start, &word_end))) {
        if (len > max_len) {
            max_len = len;
            max_word_start = word_start;
            max_word_end = word_end;
        }
    }

    printf("%d\n", max_len);
    if (max_len > 0) {
        *max_word_end = '\0';
        printf("%s\n", max_word_start);
    }

    free(str);
    return 0;
}