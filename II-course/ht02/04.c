#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum 
{
    DEFAULT_CAPACITY = 32,
    MAX_CODE_POINT_TO_IGNORE = 0x20
};

static int size_cmp;
static unsigned char *base_cmp;

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
        if (cur_pos == size_local) {
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

int cmp(const int *p1, const int *p2) {
    int s1 = *p1, s2 = *p2;
    int i = 0;
    while (i < size_cmp && base_cmp[s1] == base_cmp[s2]) {
        i++;
        s1 = (s1 + 1) % size_cmp;
        s2 = (s2 + 1) % size_cmp;
    }
    if (base_cmp[s1] == base_cmp[s2]) {
        return 0;
    }
    return base_cmp[s1] > base_cmp[s2] ? 1 : -1;
}

void append_code_point(char *dst, const char *src) {
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

    int *offsets = malloc(len * sizeof(*offsets));
    if (offsets == NULL) {
        return 0;
    }

    int pos = 0;
    for (int i = 0; i < len; i++) {
        offsets[i] = pos;
        pos += code_point_to_bytes(str[pos]);
    }

    size_cmp = size;
    base_cmp = str;
    qsort(offsets, len, sizeof(*offsets), (int(*)(const void *, const void *))cmp);

    char *ret = calloc(size + 1, sizeof(*ret));
    if (ret == NULL) {
        return 0;
    }
    pos = 0;
    // Go through matrix lines
    for (int i = 0; i < len; i++) {
        int j = size - 1;
        while (!is_code_point_start(str[(offsets[i] + j) % size])) {
            j--;
        }
        append_code_point(ret + pos, str + (offsets[i] + j) % size);
        pos += code_point_to_bytes(str[(offsets[i] + j) % size]);
    }
    ret[size] = '\0';

    printf("%s\n", ret);
    free(str);
    free(ret);

    return 0;
}