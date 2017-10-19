/*
Submit a solution for up05-4-unix/filesystem/traverse-2

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/03 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up05-4: unix/filesystem/traverse-2

В аргументах командной строки передается путь к каталогу. Этот каталог является корнем поддерева файловой системы.

Обойдите рекурсивно все подкаталоги и на стандартный поток вывода напечатайте команды 'cd', с помощью которых можно обойти все подкаталоги и вернуться в исходный каталог. Каждая команда cd может переходить на один каталог вверх/вниз по иерархии.

Никакого экранирования спецсимволов не требуется.

В одном каталоге подкаталоги должны просматриваться в лексикографическом порядке без учета регистра букв. Сравнение без учета регистра букв выполняется функцией strcasecmp. Каталог, который нельзя открыть на чтение с помощью opendir, должен быть пропущен, то есть в него не нужно входить и из него не нужно выходить.

Гарантируется, что в одном каталоге не присутствуют каталоги, отличающиеся только регистром букв.

Не используйте scandir, ftw и аналогичные функции. Не используйте chdir, fchdir. Можно использовать opendir, readdir, closedir и *stat.

Обрабатывать пути длиннее чем PATH_MAX-1 не требуется.

Например.

cd a
cd b
cd c
cd ..
cd d
cd ..
cd ..
cd ..
*/
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