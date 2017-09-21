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
