/*
Submit a solution for kr02-1-kr02-1

Full score: 200
Run penalty:    10
Time limit: 2 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/28 08:45:00
Deadline:   2017/11/28 10:25:00
Problem kr02-1: kr02-1

На стандартный поток ввода подается содержимое файла, описывающего структуру 32-битного виртуального адресного пространства процесса. Пример такого файла приведен ниже:

08048000-08053000 r-xp 00000000 08:03 18877      /usr/bin/cat
08053000-08054000 r--p 0000a000 08:03 18877      /usr/bin/cat
08054000-08055000 rw-p 0000b000 08:03 18877      /usr/bin/cat
091e3000-09204000 rw-p 00000000 00:00 0          [heap]
4f2d0000-4f2ef000 r-xp 00000000 08:03 1857       /usr/lib/ld-2.15.so
4f2ef000-4f2f0000 r--p 0001e000 08:03 1857       /usr/lib/ld-2.15.so
4f2f0000-4f2f1000 rw-p 0001f000 08:03 1857       /usr/lib/ld-2.15.so
4f2f7000-4f4a2000 r-xp 00000000 08:03 1858       /usr/lib/libc-2.15.so
4f4a2000-4f4a3000 ---p 001ab000 08:03 1858       /usr/lib/libc-2.15.so
4f4a3000-4f4a5000 r--p 001ab000 08:03 1858       /usr/lib/libc-2.15.so
4f4a5000-4f4a6000 rw-p 001ad000 08:03 1858       /usr/lib/libc-2.15.so
4f4a6000-4f4a9000 rw-p 00000000 00:00 0 
b75c0000-b77c0000 r--p 00000000 08:03 57661      /usr/lib/locale/locale-archive
b77c0000-b77c1000 rw-p 00000000 00:00 0 
b77d9000-b77da000 rw-p 00000000 00:00 0 
b77da000-b77db000 r-xp 00000000 00:00 0          [vdso]
bf819000-bf83a000 rw-p 00000000 00:00 0          [stack]
Первый столбец — виртуальный адрес начала отображенной области виртуальной памяти, второй столбец — адрес конца отображенной области, третий столбец — права доступа.

На стандартный поток вывода напечатайте два числа: размер отображенной виртуальной памяти, доступной процессу на запись, и размер отображенной виртуальной памяти, доступной процессу на исполнение.

Например, для данных, приведенных выше, ваша программа должна вывести:

303104
1925120
*/
#include<stdio.h>
#include<string.h>

enum
{
    BUF_SIZE = 32
};

int read_line(char *buf, int size, FILE *f) {
    if (!fgets(buf, size, f)) {
        return 0;
    }
    if (buf[strlen(buf) - 1] != '\n') {
        int c;
        do {
            c = fgetc(f);
        } while (c != '\n' && c != EOF);
    }
    return 1;
}

int main(void) {
    unsigned write_count = 0, exec_count = 0;
    char buf[BUF_SIZE];
    while(read_line(buf, sizeof(buf), stdin)) {
        unsigned a, b;
        char buf2[BUF_SIZE];
        sscanf(buf, "%x-%x %s", &a, &b, buf2);
        if (buf2[1] == 'w') {
            write_count += b - a;
        }
        if (buf2[2] == 'x') {
            exec_count += b - a;
        }
    }
    printf("%u\n%u\n", write_count, exec_count);
}