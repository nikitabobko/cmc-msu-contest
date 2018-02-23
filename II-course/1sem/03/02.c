/*
Submit a solution for up03-2-unix/files/write-be-16-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/19 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up03-2: unix/files/write-be-16-1

На стандартный поток ввода программы подается последовательность целых чисел типа unsigned short в текстовом виде. В файл, имя которого указано в аргументе командной строки, запишите эту последовательность чисел в бинарном виде. Числа должны записываться в формате Big Endian (старшая половина первой).

Выходной файл должен быть создан с правами только на чтение и запись только для владельца, если он не существовал.

Для записи выходного файла используйте POSIX API.

Для записи чисел используйте системный вызов write, в который передается буфер размера 2.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int
main(int argc, const char *argv[]) 
{
    if (argc != 2) {
        return 1; 
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        return 1;
    }
    unsigned short number;
    char buf[sizeof(number)];
    while (scanf("%hu", &number) == 1) {
        for (int i = sizeof(buf) - 1; i >= 0; i--) {
            buf[i] = number;
            number >>= CHAR_BIT;
        }
        write(fd, buf, sizeof(buf));
    }
    return 0;
}
