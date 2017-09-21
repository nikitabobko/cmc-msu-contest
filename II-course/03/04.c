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
        file->last_time_read = read(fileno(stdin), file->buf, BUF_SIZE);
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
        char sign = 1;
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