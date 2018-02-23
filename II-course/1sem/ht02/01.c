/*
Submit a solution for ht02-1-c/strings/getline2-1

Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/10/12 14:00:00
Date penalty:	2017/12/31 0-50/14d
Problem ht02-1: c/strings/getline2-1

Написать функцию getline2, которая считывает строку текста (до символа \n) из заданного дескриптора потока.

char *getline2(FILE *f);
Функция считывает последовательность байтов неограниченной длины до символа ‘\n’ или до конца файла. В последовательности байтов не может встречаться байт ‘\0’. Функция возвращает указатель на строку (то есть завершающуюся байтом 0), выделенную в динамической памяти. В конце строки сохраняется байт ‘\n’, если он был считан из потока. Если из потока не был считан ни один байт но сразу был получен признак конца файла, функция getline должна вернуть NULL.

Используйте динамические массивы.
*/
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
            if (line == NULL) {
                return NULL;
            }
        }
        if (cur_pos + 1 >= size) {
            size *= 2;
            char *ptr = realloc(line, size * sizeof(*line));
            if (ptr == NULL) {
                free(line);
                return NULL;
            }
            line = ptr;
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