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
            closedir(dir);
            return strdup(dd->d_name);
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
        local_buf[0] = '/';
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
    char cwd_path[PATH_MAX];
    if (getcwd_internal(cwd_path, sizeof(cwd_path)) < 0) {
        exit(1);
    }
    if (buf != NULL && size > 0) {
        *buf = '\0';
    }
    ssize_t ret;
    if (fchdir(fd) || (ret = getcwd_internal(buf, size)) < 0) {
        ret = -1;
    }
    if (chdir(cwd_path)) {
        exit(1);
    }
    return ret;
}

int main(int argc, char const *argv[]) {
    char path[15];
    printf("%zi %s\n", getcwd2(dirfd(opendir("/home")), path, sizeof(path)), path);
    char p[PATH_MAX];
    printf("%s\n", getcwd(p, sizeof(p)));
    return 0;
}