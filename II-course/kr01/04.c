/*
Submit a solution for ku01-4-kr01-4 (upsolving)

Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/10/17 16:00:00
Date penalty:	2017/12/31 0-50/14d
Problem ku01-4: kr01-4 (upsolving)

Некоторый бинарный файл хранит односвязный список структур переменного размера, схематически определенных следующим образом:

struct FileEntry
{
    unsigned short len;
    int offset;
    char str[];
};
Поле offset содержит смещение в байтах от начала текущего элемента списка до начала следующего элемента списка. У последнего элемента списка поле offset равно 0. Поле располагается в файле по смещению 2 от начала элемента списка.

Поле len — это длина строки, хранящейся в поле str. Длина строки всегда больше 0. Поле располагается в файле по смещению 0 от начала элемента списка.

Поле str — это строка. Строка не содержит нулевых байтов в середине. Байт-терминатор '\0' не хранится, то есть len — это длина значащей части строки, и ровно столько места занимает строка. Поле располагается в файле по смещению 6 от начала элемента списка.

Пустой бинарный файл допускается и считается, что он не содержит ни одной строки. Список всегда начинается в начале файла (смещение 0). Все целые числа хранятся в формате little-endian.

Программе в аргументе командной строки передается имя бинарного файла. Программа должна вывести на стандартный поток вывода лексикографически максимальную строку, хранящуюся в этом файле. В случае пустого файла не должно выводиться ничего.

Используйте интерфейс системных вызовов.

Пример бинарного входного файла: input.bin.

Правильный ответ на этом файле:

dfr
*/
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

    data->str = calloc(data->len + 1, sizeof(data->str[0]));
    if (data->str == NULL) {
        return 0;
    }
    if (!read_buf(fd, data->str, data->len)) {
        free(data->str);
        return 0;
    }
    data->str[data->len] = '\0';

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
    do {
        if (!read_entry(fd, data.offset, &data)) {
            break;
        }
        if (alphabetically_max == NULL || strcmp(data.str, alphabetically_max) > 0) {
            free(alphabetically_max);
            alphabetically_max = data.str;
        }
    } while (data.offset != 0);
    if (alphabetically_max != NULL) {
        printf("%s\n", alphabetically_max);
        free(alphabetically_max);
    }
    close(fd);
    return 0;
}