#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>

enum 
{
    BUF_SIZE = 6144,
    NUMERICAL_BASE_SYSTEM = 10
};

void child(const char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        exit(1);
    }
    
    char buf[BUF_SIZE];
    fgets(buf, sizeof(buf), f);
    fclose(f);
    size_t len = strlen(buf);
    if (len == 0) {
        exit(1);
    }
    if (buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
    
    execlp(buf, buf, NULL);
    _exit(1);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        return 0;
    }
    unsigned count = 0;
    int n = strtol(argv[1], NULL, NUMERICAL_BASE_SYSTEM);
    int last_parallel = n + 1 < argc - 1 ? n + 1 : argc - 1;
    for (int i = 2; i <= last_parallel; i++) {
        if (!fork()) {
            child(argv[i]);   
        }
    }
    for (int i = 2; i <= last_parallel; i++) {
        int status;
        wait(&status);
        count += (WIFEXITED(status) && !WEXITSTATUS(status));
    }

    for (int i = last_parallel + 1; i < argc; i++) {
        if (!fork()) {
            child(argv[i]);
        }
        
        int status;
        wait(&status);
        count += (WIFEXITED(status) && !WEXITSTATUS(status));
    }
    
    printf("%u\n", count);
    return 0;
}