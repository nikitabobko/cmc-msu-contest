#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/time.h>

int get_random_number(void) {
    int rand_fd;
    int random;
    if ((rand_fd = open("/dev/urandom", O_RDONLY)) < 0 || 
            read(rand_fd, &random, sizeof(random)) < 0) {
        if (rand_fd > 0) {
            close(rand_fd);
        }
        struct timeval time_info;
        gettimeofday(&time_info, NULL);
        srand(time_info.tv_usec);
        return rand();    
    } else {
        close(rand_fd);
        return random;
    }
}

FILE *get_temp_file_for_python_script(int len, char path[len]) {
    const char *dir_name = getenv("XDG_RUNTIME_DIR");
    if (!dir_name) {
        dir_name = getenv("TMPDIR");
    }
    if (!dir_name) {
        dir_name = "/tmp";
    }

    int fd;
    // Searching for non existence file
    do {
        snprintf(path, len, "%s/t_script_%d.py", dir_name, get_random_number());
    } while ((fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0777)) < 0);

    return fdopen(fd, "w");
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        return 1;
    }
    char path[PATH_MAX];
    FILE *script = get_temp_file_for_python_script(sizeof(path), path);
    if (script == NULL) {    
        return 1;
    }
    fprintf(script, "%s", 
            "#!/usr/bin/env python\n"
            "print(");
    for (int i = 1; i < argc; ++i) {
        if (i == argc - 1) {
            fprintf(script, "%s", argv[i]);
        } else {
            fprintf(script, "%s*", argv[i]);
        }
    }
    fprintf(script,
            ")\n"
            "import os\n"
            "os.system(\"rm %s\")\n", path);
    fclose(script);
    execl(path, path, NULL);
    remove(path);
    _exit(1);
}