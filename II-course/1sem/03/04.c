/*
Submit a solution for up03-4-unix/files/buffered-input-3

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/19 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up03-4: unix/files/buffered-input-3

На стандартном потоке ввода задается последовательность целых чисел (int). Целые числа записываются в десятичной форме со знаком + или - перед числом и разделяются одним или несколькими пробельными символами. Знак при положительном числе может опускаться.

На стандартный вывод напечатайте сумму этих чисел.

Для чтения со стандартного ввода используйте системный вызов read с размером буфера для чтения 16 байт!

Не накапливайте в памяти считываемые символы.

Не забывайте выводить символ перехода на новую строку в конце вывода!

Глобальные (и static) переменные запрещены.
*/

#include <unistd.h>
#include <stdio.h>

enum 
{
    BUF_SIZE = 16
};

typedef struct MyFile 
{
    unsigned char buf[BUF_SIZE];
    int last_time_read;
    int pos;
} MyFile;

// returns char casted to int in order to be able return EOF
int
get_next_char(MyFile *file) 
{
    if (file->pos >= file->last_time_read) {
        file->last_time_read = read(fileno(stdin), file->buf, sizeof(file->buf));
        if (file->last_time_read == -1) {
            return -2;
        }
        if (file->last_time_read == 0) {
            return EOF;
        }
        file->pos = 0;
    }
    return file->buf[file->pos++];
}

int
main(void)
{
    MyFile file;
    file.last_time_read = 0;
    file.pos = 0;
    long long sum = 0;
    int c;
    while ((c = get_next_char(&file)) != EOF && c != '\n') {
        while (c == ' ') {
            c = get_next_char(&file);
        }
        if (c == EOF || c == '\n') {
            break;
        }
        int sign = 1;
        int number = 0;
        do {
            if (c == -2) {
                return 1;
            }
            if (c == '-') {
                sign = -1;
                continue;
            }
            if (c == '+') {
                continue;
            }
            number = number * 10 + c - '0';
        } while((c = get_next_char(&file)) != ' ' && c != EOF && c != '\n');
        sum += sign * number;
    }
    printf("%lld\n", sum);
    return 0;
}