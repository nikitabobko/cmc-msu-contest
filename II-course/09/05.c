/*
Submit a solution for up09-5-unix/processes/invoke-script-1
Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/07 10:30:00
Date penalty formula:   2017/12/31 0-50/14d
Current penalty:    100
Next soft deadline: 2017/12/19 10:30:00
Problem up09-5: unix/processes/invoke-script-1
В аргументах командной строки передаются целые положительные числа произвольного размера. Число передаваемых аргументов больше или равно 1. На стандартный поток вывода напечатайте произведение этих чисел.

Для вычисления результата используйте скриптовый язык программирования, поддерживающий арифметику произвольной точности, например, python, python3.

На языке python3 умножение двух чисел выглядит следующим образом:

print(1*2)
Если записать эту программу в файл apb.py и запустить интерпретатор командой

python3 apb.py
То на стандартном потоке вывода будет получен результат:

3
Ваша программа должна создать скрипт, который не будет требовать никаких дополнительных входных данных и при запуске выведет на стандартный поток вывода произведение чисел. При запуске команды на выполнение из вашей программы не должно указываться явное имя интерпретатора, поэтому в скрипте используйте запись #!.

Не создавайте новый процесс. Ваша программа может запускаться в произвольном рабочем каталоге. Программа должна поддерживать возможность одновременной работы своих копий. Программа не должна допускать возможность изменения скрипта другими пользователями. Скрипт должен удаляться после выполнения.

Не используйте функции tmpnam, tmpnam_r, mktemp, mkstemp, tmpfile и аналогичные. В качестве пути к каталогу временных файлов используйте значение переменной окружения XDG_RUNTIME_DIR, если эта переменная не установлена, используйте значение переменной окружения TMPDIR, если и эта переменная не установлена, используйте путь /tmp.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/time.h>

int get_random_number(void) {
    int rand_fd;
    int random;
    if ((rand_fd = open("/dev/urandom", O_RDONLY)) < 0 || 
            read(rand_fd, &random, sizeof(random)) < 0) {
        if (rand_fd > 0) {
            close(rand_fd);
        }
        struct timeval time_info;
        gettimeofday(&time_info, NULL);
        srand(time_info.tv_usec);
        return rand();    
    } else {
        close(rand_fd);
        return random;
    }
}

FILE *get_temp_file_for_python_script(int len, char path[len]) {
    const char *dir_name = getenv("XDG_RUNTIME_DIR");
    if (!dir_name) {
        dir_name = getenv("TMPDIR");
    }
    if (!dir_name) {
        dir_name = "/tmp";
    }

    int fd;
    // Searching for non existence file
    do {
        snprintf(path, len, "%s/t_script_%d.py", dir_name, get_random_number());
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