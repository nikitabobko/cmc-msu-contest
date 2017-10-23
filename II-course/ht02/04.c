#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct InfoForCmp
{
    int size;
    unsigned char *base;
} InfoForCmp;

enum 
{
    DEFAULT_CAPACITY = 32,
    MAX_CODE_POINT_TO_IGNORE = 0x20,
    MAX_BYTES_IN_CODE_POINT = 6
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

char *getstr(int *size) {
    int size_local = 0, cur_pos = 0;
    char *line = NULL;
    int c;
    while ((c = getc_unlocked(stdin)) != EOF) {
        if (c <= MAX_CODE_POINT_TO_IGNORE) {
            continue;
        }
        if (line == NULL) {
            size_local = DEFAULT_CAPACITY;
            line = malloc(size_local * sizeof(*line));
            if (line == NULL) {
                return NULL;
            }
        }
        if (cur_pos + 7 >= size_local) {
            size_local += size_local / 2;
            char *ptr = realloc(line, size_local * sizeof(*line));
            if (ptr == NULL) {
                free(line);
                return NULL;
            }
            line = ptr;
        }
        line[cur_pos++] = c;
    }
    if (size != NULL) {
        *size = cur_pos;
    }
    return line;
}

int length(unsigned char *str, int size) {
    if (str == NULL) {
        return 0;
    }
    int len = 0;
    int i = 0;
    while (i < size) {
        i += code_point_to_bytes(str[i]);
        len++;
    }
    return len;
}

int is_code_point_start(unsigned char c) {
    return (c >> 6) != 0x2;
}

int cmp(unsigned const char **p1, unsigned const char **p2, InfoForCmp *info) {
    unsigned const char *s1 = *p1, *s2 = *p2;
    int i = 0;
    while (i < info->size && *s1 == *s2) {
        i++;
        s1 = (s1 - info->base + 1) % info->size + info->base;
        s2 = (s2 - info->base + 1) % info->size + info->base;
    }
    return *s1 - *s2;
}

void append_code_point(char *dst, char *src) {
    for (int i = 0; i < code_point_to_bytes(*src); i++) {
        dst[i] = src[i];
    }
}

int main(int argc, char const *argv[]) {
    int size;
    char *str = getstr(&size);
    if (str == NULL) {
        return 0;
    }
    int len = length(str, size);

    char **matrix = malloc(len * sizeof(*matrix));
    if (matrix == NULL) {
        return 0;
    }

    int pos = 0;
    for (int i = 0; i < len; i++) {
        matrix[i] = str + pos;
        pos += code_point_to_bytes(str[pos]);
    }

    InfoForCmp info;
    info.size = size;
    info.base = str;
    qsort_r(matrix, len, sizeof(*matrix), (int(*)(const void *, const void *, void *))cmp, &info);

    char *ret = calloc(size + 1, sizeof(*ret));
    pos = 0;
    // Go through matrix lines
    for (int i = 0; i < len; i++) {
        int j = size - 1;
        while (!is_code_point_start(str[(matrix[i] - str + j) % size])) {
            j--;
        }
        append_code_point(ret + pos, str + (matrix[i] - str + j) % size);
        pos += code_point_to_bytes(str[(matrix[i] - str + j) % size]);
    }

    printf("%s\n", ret);
    free(str);
    free(ret);

    return 0;
}