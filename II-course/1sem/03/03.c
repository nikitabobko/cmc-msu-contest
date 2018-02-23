/*
Submit a solution for up03-3-unix/files/copy-file-fd-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/19 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up03-3: unix/files/copy-file-fd-1

Напишите функцию

void copy_file(int in_fd, int out_fd);
Функция должна скопировать все данные из файлового дескриптора in_fd в файловый дескриптор out_fd.

Для буферизации ввода используйте локальный буфер размера 4096 байт.

Используйте POSIX API работы с файлами.

Помните, что read не обязан всегда считывать ровно то количество байт, которое было запрошено.

Помните, что write не обязан всегда записывать ровно то количество байт, которое было запрошено.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum 
{
    LOC_BUF_SIZE = 4096
};

void 
copy_file(int in_fd, int out_fd) 
{
    if (in_fd == -1 || out_fd == -1) {
        return;
    }
    char buf[LOC_BUF_SIZE];
    ssize_t how_many_transfered;
    while ((how_many_transfered = read(in_fd, buf, sizeof(buf)))) {
        if (how_many_transfered == -1) {
            return;
        }
        char *pos = buf;
        while (how_many_transfered) {
            ssize_t how_many_written = write(out_fd, pos, how_many_transfered);
            if (how_many_written == -1) {
                return;
            }
            how_many_transfered -= how_many_written;
            pos += how_many_written;
        }
    }
}