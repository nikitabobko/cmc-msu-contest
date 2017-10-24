#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


int main(int argc, const char *argv[]) {
    if (argc != 4) {
        return 1;
    }
    unsigned const char *dir_name = argv[1];
    int64_t limit = strtol(argv[2], NULL, 10);
    unsigned const char *suffix = argv[3];
    int suffix_len = strlen(suffix);

    int64_t sum = 0;
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        return 1;
    }
    struct dirent *dd;
    char file_name[PATH_MAX] = {};
    off_t max_file_name_size = 0;
    while ((dd = readdir(dir))) {
        char file[PATH_MAX];
        snprintf(file, sizeof(file), "%s/%s", dir_name, dd->d_name);
        struct stat info;
        lstat(file, &info);
        if (S_ISREG(info.st_mode)) {
            sum += info.st_size;
            if (info.st_size > max_file_name_size && 
                    strcmp(dd->d_name + strlen(dd->d_name) - suffix_len, suffix) == 0 && 
                    (*file_name == '\0' || strcmp(file_name, dd->d_name) > 0)) {
                strcpy(file_name, dd->d_name);
            }
        }
    }

    if (sum > limit && *file_name != '\0') {
        printf("%s\n", file_name);
    }
    return 0;
}