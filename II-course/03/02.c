#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

char 
is_little_endian(void)
{
    unsigned short a = 0x1;
    return *((char *)&a);
}

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
        if (is_little_endian()) {
            buf[0] = *(((char *)&number) + 1);
            buf[1] = *((char *)&number);    
        } else {
            *((unsigned short *)buf) = number;
        }
        write(fd, buf, sizeof(buf));
    }
    return 0;
}
