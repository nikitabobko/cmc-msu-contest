#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        return 0;
    }
    const char *dir_name = argv[1];
    int64_t limit = strtol(argv[2], NULL, 10);
    const char *suffix = argv[3];
    int suffix_len = strlen(suffix);

    int64_t sum = 0;
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        return 0;
    }
    struct dirent *dd;
    char *file_name = NULL;
    off_t max_size = 0;
    while ((dd = readdir(dir))) {
        char file[PATH_MAX + 1];
        snprintf(file, sizeof(file), "%s/%s", dir_name, dd->d_name);
        struct stat info;
        if (lstat(file, &info)) {
            continue;
        }
        if (S_ISREG(info.st_mode)) {
            sum += info.st_size;
            if (info.st_size > max_size &&
                    strcmp(dd->d_name + strlen(dd->d_name) - suffix_len, suffix) == 0 &&
                    (file_name == NULL || strcmp(file_name, dd->d_name) > 0)) {
                free(file_name);
                file_name = strdup(dd->d_name);
                max_size = info.st_size;
            }
        }
    }

    if (sum > limit && file_name != NULL) {
        printf("%s\n", file_name);
    }
    free(file_name);
    closedir(dir);
    return 0;
}