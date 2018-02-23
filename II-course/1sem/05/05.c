/*
Submit a solution for up05-5-c/binary-files/binary-reverse-3

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/03 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up05-5: c/binary-files/binary-reverse-3

В аргументах командной строки передается имя бинарного файла произвольного доступа и 32-битное знаковое целое число A. Бинарный файл содержит структуры Data, описанные ниже.

struct Data
{
    int16_t x; // 2 байта,
    int64_t y; // 8 байт
};
Размер каждой структуры в файле - 10 байт, структуры в файле располагаются подряд.

Числа хранятся в представлении Little-Endian. Ваша программа должна корректно работать на платформах с любым порядком байт.

Преобразуйте файл следующим образом:

Измените порядок следования структур на противоположный, то есть первый элемент становится последним, второй - предпоследним и т. д.
Для каждой структуры модифицируйте значение поля y следующим образом: y += x * A. В случае арифметического переполнения выводите сообщение об ошибке и завершайте работу с кодом 3.
Хранить в памяти более двух структур не разрешается. Для ввода-вывода в файле использовать низкоуровневые средства.

Напишите функции marshall и unmarshall, которые преобразовывают данные перед записью в файл. Функции имеют следующие прототипы:

void marshall(unsigned char *out, const struct Data *in);
void unmarshall(struct Data *out, const unsigned char *in);
Функция marshall преобразовывает данные, хранящиеся в структуре в параметре in в локальном представлении данных, в буфер требуемого размера, подготовленный для записи в файл. Функция unmarshall выполняет обратные действия.

Контролируйте корректность операций чтения и записи. В случае ошибки выведите сообщение об ошибке и завершите работу с кодом 2.


*/
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

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
int read_write_buf(int fd, unsigned char *buf, size_t size, ssize_t (*op)(int, void *, size_t)) {
    int i = 0;
    do {
        errno = 0;
        ssize_t bytes = op(fd, buf + i, size - i);
        if (bytes == -1) {
            printf("%s\n", strerror(errno));
            return 0;
        }
        i += bytes;
    } while (i != size);
    return 1;
}

int64_t from_little_endian(unsigned const char *buf, int size) {
    int64_t ret = 0;
    for (int i = size - 1; i >= 0; i--) {
        ret <<= CHAR_BIT;
        ret |= buf[i];
    }
    return ret;
}

// fills only first size bytes in little endian format
void to_little_endian(uint64_t number, int size, unsigned char *buf) {
    for (int i = 0; i < size; i++) {
        buf[i] = number;
        number >>= CHAR_BIT;
    }
}

// returns 1 if all is ok; otherwise 0
int read_entry(int fd, int index, Data *data) {
    lseek(fd, index * ENTRY_IN_BYTES, SEEK_SET);

    unsigned char x_buf[sizeof(data->x)];
    if (!read_write_buf(fd, x_buf, sizeof(x_buf), read)) {
        return 0;
    }
    data->x = from_little_endian(x_buf, sizeof(x_buf));

    unsigned char y_buf[sizeof(data->y)];
    if (!read_write_buf(fd, y_buf, sizeof(y_buf), read)) {
        return 0;
    }
    data->y = from_little_endian(y_buf, sizeof(y_buf));

    return 1;
}

int write_entry(int fd, int index, const Data *data) {
    lseek(fd, index * ENTRY_IN_BYTES, SEEK_SET);

    unsigned char x_buf[sizeof(data->x)];
    to_little_endian(data->x, sizeof(data->x), x_buf);
    if (!read_write_buf(fd, x_buf, sizeof(x_buf), (ssize_t (*)(int, void *, size_t)) write)) {
        return 0;
    }

    unsigned char y_buf[sizeof(data->y)];
    to_little_endian(data->y, sizeof(data->y), y_buf);
    if (!read_write_buf(fd, y_buf, sizeof(y_buf), (ssize_t (*)(int, void *, size_t)) write)) {
        return 0;
    }

    return 1;
}

// returns 1 if no overflow; 0 otherwise
int process_entry(Data *data, int32_t a) {
    errno = 0;
    int64_t y;
    if (__builtin_mul_overflow(data->x, a, &y) || __builtin_add_overflow(data->y, y, &(data->y))) {
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
    if (sscanf(argv[2], "%d", &a) != 1) {
        return 1;
    }
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
    if (entries % 2) {
        Data data;
        if (!read_entry(fd, entries / 2, &data)) {
            return 2;
        }
        if (!process_entry(&data, a)) {
            return 3;
        }
        if (!write_entry(fd, entries / 2, &data)) {
            return 2;
        }
    }
    close(fd);
    return 0;
}