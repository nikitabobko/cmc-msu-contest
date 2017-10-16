#include <stdio.h>
#include <stdlib.h>

enum 
{
    DEFAULT_CAPACITY = 16
};

char *getline2(FILE *f) {
    if (f == NULL) {
        return NULL;
    }
    int size = 0, cur_pos = 0;
    char *line = NULL;
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (line == NULL) {
            size = DEFAULT_CAPACITY;
            line = malloc(size * sizeof(*line));
        }
        if (cur_pos + 1 >= size) {
            size *= 2;
            line = realloc(line, size * sizeof(*line));
        }
        line[cur_pos++] = c;
        if (c == '\n') {
            break;
        }
    }
    if (line != NULL && line[cur_pos - 1] != '\0') {
        line[cur_pos] = '\0';
    }
    return line;
}