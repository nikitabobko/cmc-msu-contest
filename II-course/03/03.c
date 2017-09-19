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