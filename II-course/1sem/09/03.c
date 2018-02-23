/*
Submit a solution for up09-3-unix/processes/mysystem-1

Full score: 100
Run penalty:    10
Input file name:    input
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/07 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up09-3: unix/processes/mysystem-1

Напишите функцию:

int mysys(const char *str);
Функция должна работать аналогично функции system, то есть создавать процесс, запускать на выполнение строку str и дожидаться результата.

Строка str может содержать несколько аргументов, разделенных пробелами.

Функция mysys должна возвращать -1 в случае, если новый процесс не может быть создан. Если выполнение str завершилось с кодом завершения 0-127, функция должна возвращать это значение. Если выполнение str завершилось из-за сигнала K, то функция должна возвращать значение 128 + K.

Для интерпретации строки str должен использовать стандартный шелл /bin/sh. (sh -c CMD).

Сама функция mysys не должна ничего выводить ни на стандартный поток ошибок, ни на стандартный поток вывода.

Функция mysys должна корректно работать независимо от использования основным процессом других функций создания процессов и функций ввода-вывода.
*/
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int mysys(const char *str) {
    pid_t pid = fork();
    if (!pid) {
        execlp("sh", "sh", "-c", str, NULL);
        _exit(1);
    } else if (pid < 0) {
        return -1;
    }
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) >= 0 && WEXITSTATUS(status) <= 127) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return WTERMSIG(status) + 128;
    }
    return -1;
}
