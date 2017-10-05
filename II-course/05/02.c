/*
Submit a solution for up05-2-unix/filesystem/filter-dir-6

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/03 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up05-2: unix/filesystem/filter-dir-6

В аргументах командной строки задается путь к каталогу.

На стандартный поток вывода напечатайте суммарный размер всех файлов в заданном каталоге (без подкаталогов), удовлетворяющих следующему условию:

Файл регулярный.
Его владельцем является текущий пользователь.
Его имя начинается с заглавной латинской буквы.
Для получения идентификатора пользователя процесса используйте системный вызов getuid.

Символические ссылки прослеживайте до соответствующих файлов (используйте соответствующий системный вызов семейства *stat).

Не используйте системные вызовы, меняющие текущий каталог процесса.


*/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 1;
    }
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        return 1;
    }
    struct dirent *dd;
    long long sum = 0;
    while ((dd = readdir(dir))) {
        char file_name[PATH_MAX];
        snprintf(file_name, sizeof(file_name), "%s/%s", argv[1], dd->d_name);
        struct stat s;
        if (stat(file_name, &s) == 0 && S_ISREG(s.st_mode) && getuid() == s.st_uid && 
            isupper(dd->d_name[0])) {
            sum += s.st_size;
        }
    }
    printf("%lld\n", sum);
    return 0;
}