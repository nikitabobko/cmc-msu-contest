#include <fcntl.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <unistd.h>

ssize_t getcwd2(int fd, char *buf, size_t size) {
    char fd_symlink[PATH_MAX];
    pid_t pid = getpid();
    snprintf(fd_symlink, sizeof(fd_symlink), "/proc/%d/fd/%d", pid, fd);
    readlink(fd_symlink, buf, size);
    buf[size - 1] = '\0';
}

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    char file[PATH_MAX];
    getcwd2(fd, file, sizeof(file));
    printf("%s\n", file);
    return 0;
}