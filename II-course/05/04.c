#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

enum 
{
    BUF_SIZE = 32
};

int cmp(const char **p1, const char **p2) {
    return strcasecmp(*p1, *p2);
}

void process_dir(DIR *dir, const char *path) {
    if (dir == NULL || path == NULL) {
        return;
    }
    char **files = calloc(BUF_SIZE, sizeof(*files));
    int size = 0;
    int capacity = BUF_SIZE;
    struct dirent *dd;
    while ((dd = readdir(dir))) {
        if (strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            char file[PATH_MAX];
            int file_path_len = snprintf(file, sizeof(file), "%s/%s", path, dd->d_name);

            struct stat s;
            if (!access(file, R_OK) && !lstat(file, &s) && S_ISDIR(s.st_mode) && 
                    file_path_len < sizeof(file)) {
                if (size == capacity) {
                    capacity *= 2;
                    files = realloc(files, capacity * sizeof(*files));
                }
                files[size++] = strdup(dd->d_name);
            }
        }
    }
    closedir(dir);
    qsort(files, size, sizeof(*files), (int(*)(const void *, const void *))cmp);
    for (int i = 0; i < size; i++) {
        char file[PATH_MAX];
        snprintf(file, sizeof(file), "%s/%s", path, files[i]);
        DIR *child_dir = opendir(file);
        if (child_dir != NULL) {
            printf("cd %s\n", files[i]);
            process_dir(child_dir, file);
            printf("cd ..\n");
        }
        free(files[i]);
    }
    free(files);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 1;
    }
    if (strlen(argv[1]) > PATH_MAX - 1) {
        return 0;
    }
    process_dir(opendir(argv[1]), argv[1]);
    return 0;
}