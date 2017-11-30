/*
Submit a solution for ht03-1-unix/mmap/mmap-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/31 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem ht03-1: unix/mmap/mmap-1

Программе задаются три аргумента командной строки.

Путь path к регулярному текстовому файлу.
Номер line1 первой строки в интервале строк.
Номер line2 последней строки в интервале строк.
Номера line1 и line2 — положительные знаковые целые числа, представимые типом long.

Программа должна выделить в файле path строки текста со строки line1 включительно по строку line2, не включая её. Первая строка текста в файле имеет номер 1. Строки текста должны быть выведены на стандартный поток вывода в обратном порядке. Каждая строка текста должна завершаться строго одним символом '\n'.

Если line1 ≥ line2, или ни одна строка в файле не попадает в указанный интервал, ничего выводиться не должно. Если в указанный интервал попадает только часть строк, должна выводиться попадающая часть строк.

Для обработки файла используйте файлы, отображаемые в память (mmap). Запрещается сохранять текст входного файла где-либо еще. Запрещается использовать функции динамического выделения памяти, а также рекурсию.

Программа должна обрабатывать все ошибочные ситуации, которые могут возникать при ее работе (некорректные входные данные, ошибки выполнения системных вызовов и т. п.). В этом случае должно быть напечатано сообщение об ошибке с указанием причины ошибки, а программа должна завершиться с кодом завершения 1. Аргументы командной строки должны проверяться на корректность, даже если они не будут использоваться (например, номера строк в случае пустого файла).

Например, если заданы параметры командной строки:

./solution file.txt 4 6
А файл file.txt содержит:

1a
2a
3a
4a
5a
6a
7a
8a
9a
То на стандартный поток вывода должно быть напечатано:
5a
4a
*/
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

enum
{
    NUMERAL_SYSTEM_BASE = 10
};

size_t get_offset(long start_line, long line, const char *ptr, size_t *cur_pos) {
    long cur_line = start_line;
    size_t byte_offset = 0;
    while (cur_line < line && ptr[*cur_pos] != '\0') {
        if (ptr[(*cur_pos)++] == '\n') {
            cur_line++;
        }
        byte_offset++;
    }
    return byte_offset;
}

void print_line(const char *line) {
    while (*line != '\n' && *line != '\0') {
        putchar(*line);
        line++;
    }
    putchar('\n');
}

char *line_before(char *line, const char *point_to_line1) {
    line--;
    do {
        line--;
    } while (line + 1 > point_to_line1 && *line != '\n');
    line++;
    return line;
}

void print_lines_inverse(char *point_to_line1, size_t offset_to_line2_from_line1) {
    if (offset_to_line2_from_line1 == 0) {
        return;
    }
    char *ptr = point_to_line1 + offset_to_line2_from_line1;
    ptr = line_before(ptr, point_to_line1);
    while (ptr >= point_to_line1) {
        print_line(ptr);
        ptr = line_before(ptr, point_to_line1);
    }
}

int main(int argc, char const **argv) {
    if (argc != 4) {
        fprintf(stderr, "There should exact 3 arguments\n");
        return 1;
    }
    const char *path = argv[1];

    char *endptr1, *endptr2;
    errno = 0;
    long line1 = strtol(argv[2], &endptr1, NUMERAL_SYSTEM_BASE);
    long line2 = strtol(argv[3], &endptr2, NUMERAL_SYSTEM_BASE);
    if (errno || *endptr1 != '\0' || *endptr2 != '\0') {
        fprintf(stderr, "Error while parsing arguments\n");
        return 1;
    }
    if (line1 <= 0 || line2 <= 0) {
        fprintf(stderr, "Arguments should be positive\n");
        return 1;
    }

    int ret_code = 0;
    // Use goto to correctly free resources
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while opening the file\n");
        goto finally;
    }

    errno = 0;
    struct stat info;
    if (fstat(fd, &info) < 0 || errno) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while determening file size\n");
        goto finally;
    }
    off_t size = info.st_size;

    if (size == 0) {
        goto finally;
    }
    char *ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while mapping the file\n");
        goto finally;
    }

    if (line1 >= line2) {
        goto finally;
    }

    size_t cur_pos = 0;
    size_t offset_to_line1 = get_offset(1, line1, ptr, &cur_pos);
    size_t offset_to_line2_from_line1 = get_offset(line1, line2, ptr, &cur_pos);

    print_lines_inverse(ptr + offset_to_line1, offset_to_line2_from_line1);
finally:
    if (fd >= 0) {
        close(fd);
    }
    return ret_code;
}