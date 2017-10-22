#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    if (line != NULL) {
        line[cur_pos++] = '\0';
    }
    if (size != NULL) {
        *size = cur_pos;
    }
    return line;
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

char *strdup_with_shift(const char *str, int size) {
    if (str == NULL) {
        return NULL;
    }
    char *ret = calloc(size, sizeof(*ret));
    int first_code_point_bytes = code_point_to_bytes(*str);
    strcpy(ret, str + first_code_point_bytes);

    for (int i = 0; i < first_code_point_bytes; i++) {
        ret[size - 2 - i] = str[first_code_point_bytes - 1 - i];
    }
    ret[size - 1] = '\0';

    return ret;
}

int is_code_point_start(unsigned char c) {
    return (c >> 6) != 0x2;
}

int cmp(const char **p1, const char **p2) {
    return strcmp(*p1, *p2);
}

int main(int argc, char const *argv[]) {
    int size;
    char *str = getstr(&size);
    if (str == NULL) {
        return 0;
    }
    int len = length(str);

    char **matrix = malloc(len * sizeof(*matrix));
    if (matrix == NULL) {
        return 0;
    }

    matrix[0] = str;
    for (int i = 1; i < len; i++) {
        matrix[i] = strdup_with_shift(matrix[i-1], size);
        if (matrix[i] == NULL) {
            return 0;
        }
    }

    qsort(matrix, len, sizeof(*matrix), (int(*)(const void *, const void *))cmp);

    str = matrix[0];
    int pos = 0;
    for (int i = 0; i < len; i++) {
        int j = size - 1;
        while (matrix[i][j] == '\0' || !is_code_point_start(matrix[i][j])) {
            j--;
        }
        if (i == 0) {
            char t[MAX_BYTES_IN_CODE_POINT + 1] = {};
            strcpy(t, matrix[i] + j);
            strcpy(str + pos, t);
        } else {
            strcat(str + pos, matrix[i] + j);    
        }
        pos += code_point_to_bytes(str[pos]);
        if (i != 0) {
            free(matrix[i]);
        }
    }
    free(matrix);

    printf("%s\n", str);
    free(str);

    return 0;
}