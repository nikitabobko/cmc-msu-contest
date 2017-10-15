#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

// returns -1 if error occur; 1 if file should be skipped and 0 if shouldn't
int skip_file(const char *path, struct dirent *dd) {
    struct stat s;
    char file[PATH_MAX];
    snprintf(file, sizeof(file), "%s/%s", path, dd->d_name);
    if (lstat(file, &s) == -1) {
        return -1;
    }
    return !S_ISDIR(s.st_mode) || strcmp(dd->d_name, ".") == 0 || strcmp(dd->d_name, "..") == 0 || 
            access(file, R_OK);
}

int num_of_files_in_dir(const char *path, DIR *dir) {
    int count = 0;
    struct dirent *dd;
    while ((dd = readdir(dir))) {
        int skip = skip_file(path, dd);
        count += !skip;
    }
    rewinddir(dir);
    return count;
}

int cmp(const char **s1, const char **s2) {
    return strcasecmp(*s1, *s2);
}

void process_dir(DIR *dir, const char *path) {
    if (dir == NULL) {
        return;
    }
    int num = num_of_files_in_dir(path, dir);
    if (num < 0) {
        return;
    }
    char **files = calloc(num, sizeof(*files));
    if (files == NULL && num > 0) {
        return;
    }
    for (int i = 0; i < num; i++) {
        struct dirent *dd = readdir(dir);
        if (!dd) {
            return;
        }
        int skip = skip_file(path, dd);
        if (!skip) {
            files[i] = calloc(strlen(dd->d_name) + 1, sizeof(dd->d_name[0]));
            strcpy(files[i], dd->d_name);
        } else {
            i--;
        }
    }
    qsort(files, num, sizeof(*files), (int(*)(const void *, const void *)) cmp);
    for (int i = 0; i < num; i++) {
        char file[PATH_MAX];
        int file_path_len = snprintf(file, sizeof(file), "%s/%s", path, files[i]);
        
        DIR *child_dir = opendir(file);
        if (child_dir && file_path_len < sizeof(file)) {
            printf("cd %s\n", files[i]);
            process_dir(child_dir, file);
            printf("cd ..\n");
        }
        
        free(files[i]);
    }
    free(files);
    closedir(dir);
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