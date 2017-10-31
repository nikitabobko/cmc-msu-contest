#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    NUMERAL_SYSTEM_BASE = 10
};

void get_offset(long start_line, long line, FILE *file, long *offset_to_line_in_bytes) {
    long cur_line = start_line;
    long byte_offset = 0;
    int c;
    while (cur_line < line && (c = fgetc(file)) != EOF) {
        cur_line += c == '\n';
        byte_offset++;
    }
    *offset_to_line_in_bytes = byte_offset;
}

void print_line(char *line) {
    while (*line != '\n' && *line != '\0') {
        putchar(*line);
        line++;
    }
    putchar('\n');
}

void print_lines_inverse(char *point_to_line1, long offset_to_line2_from_line1) {
    char *ptr = point_to_line1 + offset_to_line2_from_line1 - 2;
    while (ptr + 1 > point_to_line1 && *ptr != '\n') {
        ptr--;
    }
    ptr++;
    while (ptr >= point_to_line1) {
        print_line(ptr);
        ptr -= 2;
        while (ptr + 1 > point_to_line1 && *ptr != '\n') {
            ptr--;
        }
        ptr++;
    }
}

void my_exit(FILE *file_to_close, int ret_code) {
    fclose(file_to_close);
    exit(ret_code);
}

int main(int argc, char const **argv) {
    errno = 0;
    if (argc != 4) {
        fprintf(stderr, "There should exact 3 arguments\n");
        return 1;
    }
    const char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file == NULL || errno) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    char *endptr1, *endptr2;
    long line1 = strtol(argv[2], &endptr1, NUMERAL_SYSTEM_BASE);
    long line2 = strtol(argv[3], &endptr2, NUMERAL_SYSTEM_BASE);
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        fprintf(stderr, "%s\n", errno == 0 ? "Error while parsing arguments\n" : strerror(errno));
        my_exit(file, 1);
    }
    if (line1 <= 0 || line2 <= 0) {
        fprintf(stderr, "Line number cannot be non postitive\n");
        my_exit(file, 1);
    }

    if (line1 >= line2) {
        my_exit(file, 0);
    }

    long offset_to_line1, offset_to_line2_from_line1;
    get_offset(1, line1, file, &offset_to_line1);
    get_offset(line1, line2, file, &offset_to_line2_from_line1);
    if (offset_to_line2_from_line1 == 0) {
        my_exit(file, 0);
    }

    long page_size = sysconf(_SC_PAGE_SIZE);
    long offset_in_page_to_line1 = offset_to_line1 % page_size;
    char *ptr = mmap(NULL, offset_to_line2_from_line1, PROT_READ, MAP_PRIVATE,
                     fileno(file), offset_to_line1 - offset_in_page_to_line1);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "%s\n", strerror(errno));
        my_exit(file, 1);
    }

    print_lines_inverse(ptr + offset_in_page_to_line1, offset_to_line2_from_line1);
    my_exit(file, 0);
    return 0;
}