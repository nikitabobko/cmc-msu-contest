#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct FileEntry
{
    unsigned short len;
    int offset;
    char *str;
} FileEntry;

enum
{
    FIRST_TWO_FIELDS_IN_BYTES = 6
};

// returns 1 if all is ok; otherwise 0
int read_buf(int fd, unsigned char *buf, size_t size) {
    int i = 0;
    do {
        ssize_t bytes = read(fd, buf + i, size - i);
        if (bytes == 0 || bytes == -1) {
            return 0;
        }
        i += bytes;
    } while (i != size);
    return 1;
}

int from_little_endian(unsigned const char *buf, int size) {
    int ret = 0;
    for (int i = size - 1; i >= 0; i--) {
        ret <<= CHAR_BIT;
        ret |= buf[i];
    }
    return ret;
}

// returns 1 if all is ok; otherwise 0
int read_entry(int fd, int offset, FileEntry *data) {
    lseek(fd, offset, SEEK_CUR);

    unsigned char len_buf[sizeof(data->len)];
    if (!read_buf(fd, len_buf, sizeof(len_buf))) {
        return 0;
    }
    data->len = from_little_endian(len_buf, sizeof(len_buf));

    unsigned char offset_buf[sizeof(data->offset)];
    if (!read_buf(fd, offset_buf, sizeof(offset_buf))) {
        return 0;
    }
    data->offset = from_little_endian(offset_buf, sizeof(offset_buf));

    data->str = calloc(data->len, sizeof(data->str[0]));
    if (data->str == NULL) {
        return 0;
    }
    if (!read_buf(fd, data->str, data->len)) {
        return 0;
    }

    lseek(fd, -(FIRST_TWO_FIELDS_IN_BYTES + data->len), SEEK_CUR);
    return 1;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 1;
    }
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        return 1;
    }
    char *alphabetically_max = NULL;
    FileEntry data;
    memset(&data, 0, sizeof(data));
    while (read_entry(fd, data.offset, &data) && data.offset != 0) {
        if (alphabetically_max == NULL || strcmp(data.str, alphabetically_max) > 0) {
            alphabetically_max = data.str;
        }
    }
    if (alphabetically_max != NULL) {
        printf("%s\n", alphabetically_max);
    }
    close(fd);
    return 0;
}