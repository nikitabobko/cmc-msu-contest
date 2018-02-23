/*
Submit a solution for ht02-5-unix/filesystem/getcwd-2

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/12 14:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ht02-5: unix/filesystem/getcwd-2

Напишите функцию:

ssize_t getcwd2(int fd, char *buf, size_t size);
Параметр fd — это открытый файловый дескриптор каталога, buf — адрес начала буфера в памяти, size — размер буфера.

Функция должна восстановить полный путь к каталогу, который открыт и связан с файловым дескриптором fd. То есть, если fd — корневой каталог, то полный путь к каталогу — "/". Если fd — не корневой каталог, то полный путь к этому каталогу начинается с "/", состоит из записей в соответствующих каталогах, являющихся каталогами, но не символическими ссылками. На конце полного пути "/" должен отсутствовать. Например, "/usr/local/bin" (без кавычек).

Если при выполнении функции не возникло ошибок при выполнении системных вызовов и библиотечных функций, функция getcwd2 должна вернуть длину строки — полного пути к каталогу (без учета \0). В противном случае функция должна вернуть отрицательное значение.

Если параметр size равен 0, то функция должна вернуть только длину пути и больше не выполнять никаких действий (не модифицировать память по указателю buf). Если size больше нуля, то по адресу buf должен быть записан восстановленный полный путь к каталогу, но не более чем size-1 значащих символов и байт \0. Байт \0 должен быть записан в любом случае.

Функция должна сохранять текущий рабочий каталог процесса, то есть текущий рабочий каталог процесса до вызова функции должен совпадать с текущим рабочим каталогом процесса после вызова функции. Файловый дескриптор с номером fd должен остаться открытым и связанным с тем же (исходным) каталогом.

Из функций и системных вызовов работы с файлами и файловой системой разрешается использовать только opendir, closedir, readdir, stat, lstat, fstat, chdir, fchdir, dirfd. Функции работы со строками и символами разрешается использовать без ограничений.
*/
#include <fcntl.h>
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

char *find_in_dir(struct stat *file_stat_to_find) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        return NULL;
    }
    struct dirent *dd;
    while ((dd = readdir(dir))) {
        struct stat buf;
        if (lstat(dd->d_name, &buf)) {
            closedir(dir);
            return NULL;
        }
        if (buf.st_dev == file_stat_to_find->st_dev && buf.st_ino == file_stat_to_find->st_ino) {
            char *ret = strdup(dd->d_name);
            closedir(dir);
            return ret;
        }
    }
    closedir(dir);
    return NULL;
}

static ssize_t getcwd_internal(char *buf, size_t size) {
    struct stat root_stat, cur;
    if (lstat("/", &root_stat) || lstat(".", &cur)) {
        return -1;
    }
    // By default we are in root directory
    char local_buf[PATH_MAX] = {}, temp[PATH_MAX];
    ssize_t ret = 1;
    if (cur.st_ino == root_stat.st_ino && cur.st_dev == root_stat.st_dev) {
        strcpy(local_buf, "/");
    }

    while (cur.st_ino != root_stat.st_ino || cur.st_dev != root_stat.st_dev) {
        chdir("..");
        char *found = find_in_dir(&cur);
        if (!found) {
            return -1;
        }
        strcpy(temp, local_buf);
        ret = snprintf(local_buf, sizeof(local_buf), "/%s%s", found, temp);
        free(found);
        if (lstat(".", &cur)) {
            return -1;
        }
    }
    if (buf != NULL) {
        snprintf(buf, size, "%s", local_buf);
    }
    return ret;
}

ssize_t getcwd2(int fd, char *buf, size_t size) {
    if (buf != NULL && size > 0) {
        *buf = '\0';
    }
    char cwd_path[PATH_MAX] = {};
    ssize_t ret;
    if (getcwd_internal(cwd_path, sizeof(cwd_path)) < 0 || fchdir(fd) || 
            (ret = getcwd_internal(buf, size)) < 0) {
        ret = -1;
    }
    if (chdir(cwd_path)) {
        ret = -1;
    }
    return ret;
}