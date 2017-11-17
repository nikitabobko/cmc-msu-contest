/*
Submit a solution for up09-2-unix/processes/cmdline-b

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/07 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up09-2: unix/processes/cmdline-b

В аргументах командной строки передаются параметры CMD FILE1 FILE2. Реализуйте запуск процесса CMD с перенаправлением стандартных потоков как в команде:

CMD < FILE1 > FILE2
Файл FILE2 должен создаваться с помощью системного вызова open с правами чтение-запись для всех категорий пользователей (владелец, группа, прочие). Если файл уже существует, его права доступа не изменяются.

Для запуска файла на выполнение используйте execlp. Не используйте umask для изменения соответствующего параметра процесса.

Процесс-родитель должен завершаться самым последним из всех процессов. Процесс-родитель всегда завершается с кодом 0.

Например, при запуске вашей программы:

  ./prog wc in.txt out.txt
В отдельном процессе должна быть запущена программа wc, ее стандартный ввод должен быть перенаправлен на чтение из файла in.txt, а стандартный вывод - на запись в файл out.txt.
*/
#include <unistd.h>
#include <stdio.h>
#include <wait.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        return 0;
    }

    pid_t pid;
    if (!(pid = fork())) {
        const char *cmd = argv[1];
        const char *in = argv[2];
        const char *out = argv[3];

        int file_in = open(in, O_RDONLY);
        int file_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_in < 0 || file_out < 0) {
            return 0;
        }

        if (dup2(file_in, fileno(stdin)) < 0 || dup2(file_out, fileno(stdout)) < 0) {
            return 0;
        }
    
        close(file_in);
        close(file_out);
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        return 0;
    }
    wait(NULL);
    return 0;
}