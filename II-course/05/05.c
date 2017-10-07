#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

enum
{
    ENTRY_IN_BYTES = 10
};

typedef struct Data
{
    int16_t x;
    int64_t y;
} Data;

int count_entries(int fd) {
    return lseek(fd, 0, SEEK_END) / ENTRY_IN_BYTES;
}

// returns 1 if all is ok; otherwise 0
int read_write_buf(int fd, char *buf, size_t size, ssize_t (*op)(int, void *, size_t)) {
    int i = 0;
    do {
        ssize_t bytes = op(fd, buf + i, size);
        if (bytes == -1) {
            return 0;
        }
        i += bytes;
    } while (i != size);
    return 1;
}

int64_t from_little_endian(const char *buf, int size) {
    int64_t ret = 0;
    for (int i = 0; i < size; i++) {
        ret <<= CHAR_BIT;
        ret |= buf[size - 1 - i];
    }
    return ret;
}

// fills only first size bytes in little endian format
void to_little_endian(uint64_t number, int size, char *buf) {
    for (int i = 0; i < size; i++) {
        buf[i] = number;
        number >>= CHAR_BIT;
    }
}

// returns 1 if all is ok; otherwise 0
int read_entry(int fd, int index, Data *data) {
    lseek(fd, index * ENTRY_IN_BYTES, SEEK_SET);

    char x_buf[sizeof(data->x)];
    if (!read_write_buf(fd, x_buf, sizeof(x_buf), read)) {
        return 0;
    }
    data->x = from_little_endian(x_buf, sizeof(x_buf));

    char y_buf[sizeof(data->y)];
    if (!read_write_buf(fd, y_buf, sizeof(y_buf), read)) {
        return 0;
    }
    data->y = from_little_endian(y_buf, sizeof(y_buf));

    return 1;
}

int write_entry(int fd, int index, const Data *data) {
    lseek(fd, index * ENTRY_IN_BYTES, SEEK_SET);

    char x_buf[sizeof(data->x)];
    to_little_endian(data->x, sizeof(data->x), x_buf);
    if (!read_write_buf(fd, x_buf, sizeof(x_buf), (ssize_t (*)(int, void *, size_t)) write)) {
        return 0;
    }

    char y_buf[sizeof(data->y)];
    to_little_endian(data->y, sizeof(data->y), y_buf);
    if (!read_write_buf(fd, y_buf, sizeof(y_buf), (ssize_t (*)(int, void *, size_t)) write)) {
        return 0;
    }

    return 1;
}

// returns 1 if no overflow; 0 otherwise
int process_entry(Data *data, int32_t a) {
    int64_t y;
    if (__builtin_mul_overflow(data->x, a, &y) || __builtin_add_overflow(data->y, y, &y)) {
        return 0;
    }
    return 1;
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        return 1;
    }
    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        return 1;
    }
    int32_t a;
    sscanf(argv[2], "%d", &a);
    int entries = count_entries(fd);
    for (int i = 0; i < entries / 2; i++) {
        Data first, second;
        if (!read_entry(fd, i, &first) || !read_entry(fd, entries - 1 - i, &second)) {
            return 2;
        }
        if (!process_entry(&first, a) || !process_entry(&second, a)) {
            return 3;
        }
        if (!write_entry(fd, i, &second) || !write_entry(fd, entries - 1 - i, &first)) {
            return 2;
        }
    }
    return 0;
}