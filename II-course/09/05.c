#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>


FILE *get_temp_file_for_python_script(int len, char path[len]) {
    const char *dir_name = getenv("XDG_RUNTIME_DIR");
    if (!dir_name) {
        dir_name = getenv("TMPDIR");
    }
    if (!dir_name) {
        dir_name = "/tmp";
    }

    unsigned i = 0;
    int fd;
    // Searching for non existence file
    do {
        snprintf(path, len, "%s/temp_python_script_%d.py", dir_name, i++);
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