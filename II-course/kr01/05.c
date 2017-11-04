/*
Submit a solution for ku01-5-kr01-5 (upsolving)

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/17 16:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ku01-5: kr01-5 (upsolving)

В аргументах командной строки передаются: путь к каталогу, максимально допустимый суммарный размер файла в каталоге (64-битное целое число) и суффикс имени файлов - непустая строка, не содержащая символ '/'.

Если суммарный размер регулярных файлов в данном каталоге превышает заданное ограничение, необходимо вывести на стандартный поток вывода имя регулярного файла самого большого размера из файлов, имя которых оканчивается на указанный суффикс, если такой файл существует. Если файлов самого большого размера несколько, выведите лексикографически минимальное имя.

Файлы, являющиеся символическими ссылками, не рассматривать при подсчете размера и нахождении максимального файла.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>

int check_suffix(const char *str, const char *suffix, int suffix_len) {
    int str_len = strlen(str);
    return suffix_len <= str_len && strcmp(str + str_len - suffix_len, suffix) == 0;
}

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        return 0;
    }
    const char *dir_path = argv[1];
    int64_t limit = strtol(argv[2], NULL, 10);
    const char *suffix = argv[3];
    int suffix_len = strlen(suffix);

    uint64_t sum = 0;
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        return 0;
    }
    struct dirent *dd;
    char *file_name = NULL;
    off_t max_size = 0;
    while ((dd = readdir(dir))) {
        char file[PATH_MAX];
        snprintf(file, sizeof(file), "%s/%s", dir_path, dd->d_name);
        struct stat info;
        if (!lstat(file, &info) && S_ISREG(info.st_mode)) {
            sum += info.st_size;
            if (info.st_size >= max_size && check_suffix(dd->d_name, suffix, suffix_len)) {
                if (info.st_size > max_size || 
                        (file_name == NULL || strcmp(file_name, dd->d_name) > 0)) {
                    free(file_name);
                    file_name = strdup(dd->d_name);
                    max_size = info.st_size;
                }
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