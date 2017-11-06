#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    NUMERAL_SYSTEM_BASE = 10
};

int get_offset(int start_line, int line, FILE *file) {
    int cur_line = start_line;
    int byte_offset = 0;
    int c;
    while (cur_line < line && (c = fgetc(file)) != EOF) {
        if (c == '\n') {
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

void print_lines_inverse(char *point_to_line1, int offset_to_line2_from_line1) {
    char *ptr = point_to_line1 + offset_to_line2_from_line1 - 1;
    do {
        ptr--;
    } while (ptr + 1 > point_to_line1 && *ptr != '\n');
    ptr++;
    while (ptr >= point_to_line1) {
        print_line(ptr);
        ptr--;
        do {
            ptr--;
        } while (ptr + 1 > point_to_line1 && *ptr != '\n');
        ptr++;
    }
}

void my_exit(FILE *file_to_close, const char *msg, int ret_code) {
    if (msg) {
        fprintf(stderr, "%s\n", msg);
    }
    if (file_to_close) {
        fclose(file_to_close);
    }
    exit(ret_code);
}

int is_regular_file(const char *path) {
    struct stat buf;
    if (lstat(path, &buf) < 0) {
        my_exit(NULL, "Error while determening whether file is regular", 1);
    }
    return S_ISREG(buf.st_mode);
}

int main(int argc, char const **argv) {
    if (argc != 4) {
        my_exit(NULL, "There should exact 3 arguments", 1);
    }
    const char *path = argv[1];
    if (!is_regular_file(path)) {
        my_exit(NULL, "File is not regular", 1);
    }
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        my_exit(NULL, "Error occurred while opening the file", 1);
    }

    char *endptr1, *endptr2;
    long line1 = strtol(argv[2], &endptr1, NUMERAL_SYSTEM_BASE);
    long line2 = strtol(argv[3], &endptr2, NUMERAL_SYSTEM_BASE);
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        my_exit(file, "Error while parsing arguments", 1);
    }
    if (line1 <= 0 || line2 <= 0) {
        my_exit(file, "Line number cannot be non postitive", 1);
    }

    if (line1 >= line2) {
        my_exit(file, NULL, 0);
    }

    int offset_to_line1, offset_to_line2_from_line1;
    offset_to_line1 = get_offset(1, line1, file);
    offset_to_line2_from_line1 = get_offset(line1, line2, file);
    if (offset_to_line2_from_line1 == 0) {
        my_exit(file, NULL, 0);
    }

    long page_size = sysconf(_SC_PAGE_SIZE);
    int offset_in_page_to_line1 = offset_to_line1 % page_size;
    char *ptr = mmap(NULL, offset_to_line2_from_line1, PROT_READ, MAP_PRIVATE,
                    fileno(file), offset_to_line1 - offset_in_page_to_line1);
    if (ptr == MAP_FAILED) {
        my_exit(file, "Error occurred while mapping the file", 1);
    }

    print_lines_inverse(ptr + offset_in_page_to_line1, offset_to_line2_from_line1);
    my_exit(file, NULL, 0);
    return 0;
}