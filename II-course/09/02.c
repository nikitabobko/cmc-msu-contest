#include <unistd.h>
#include <stdio.h>
#include <wait.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        return 0;
    }

    pid_t pid;
    if (!(pid = fork())) {
        const char *cmd = argv[1];
        const char *in = argv[2];
        const char *out = argv[3];

        int file_in = open(in, O_RDONLY);
        int file_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_in < 0 || file_out < 0) {
            return 0;
        }

        if (dup2(file_in, fileno(stdin)) < 0 || dup2(file_out, fileno(stdout)) < 0) {
            return 0;
        }
    
        close(file_in);
        close(file_out);
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        return 0;
    }
    wait(NULL);
    return 0;
}