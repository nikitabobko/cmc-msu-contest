/*
Submit a solution for up10-4-unix/pipe/pipe-4

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/14 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up10-4: unix/pipe/pipe-4

Программе в аргументах командной строки передается произвольное (возможно пустое) количество команд для выполнения. Программа должна запустить на выполнение процессы, чтобы они выполнялись в конфигурации, соответствующей команде shell:

cmd1 | cmd2 | ... | cmdN
Родительский процесс должен дождаться завершения всех созданных им процессов и сам завершить выполнение. Если все системные вызовы, исполнявшиеся родителем (например, создание процесса), завершились успешно, родитель завершается с кодом 0. В случае ошибки выполнения системного вызова (например, невозможности создания очередного процесса), родитель уничтожает уже созданные к этому моменту процессы с помощью сигнала SIGKILL и сам завершается с кодом 1 после завершения сыновей.

Команды cmd1, cmd2, ... запускайте на выполнение с помощью execlp.

В родителе одновременно может быть открыто не более 6 файловых дескрипторов.

Создаваемые процессы должны оставаться в группе процессов родителя.

Программа не должна ничего выводить на стандартный поток ошибок.

Например, если скомпилированная программа называется solution, то ее запуск с аргументами

./solution ls cat wc
должен дать такой же результат на стандартном потоке вывода, как выполнение команды shell

ls | cat | wc
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>

void kill_children(pid_t *children_pids, int children_count) {
    for (int i = 0; i < children_count; i++) {
        if (children_pids[i] > 0) {
            kill(children_pids[i], SIGKILL);
        } else {
            break;
        }
    }
    for (int i = 0; i < children_count; i++) {
        wait(NULL);
    }
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        return 0;
    }
    int stdin_fd_backup = dup(STDIN_FILENO);
    if (stdin_fd_backup < 0) {
        return 1;
    }
    pid_t *children_pids = calloc((argc - 1), sizeof(*children_pids));
    if (!children_pids) {
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        int fd[2];
        if (i != argc - 1 && pipe(fd) < 0) {
            kill_children(children_pids, argc - 1);
            return 1;
        }

        children_pids[i - 1] = fork();
        if (!children_pids[i - 1]) {
            if (i != argc - 1 && (dup2(fd[1], STDOUT_FILENO) < 0 || close(fd[1]) < 0 || 
                    close(fd[0]) < 0)) {
                exit(1);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else if (children_pids[i - 1] < 0) {
            kill_children(children_pids, argc - 1);
            return 1;   
        }

        if (i != argc - 1 && (dup2(fd[0], 0) < 0 || close(fd[0]) < 0 || 
                close(fd[1]) < 0)) {
            kill_children(children_pids, argc - 1);
            return 1;
        }
    }
    if (dup2(stdin_fd_backup, 0) < 0 || close(stdin_fd_backup) < 0) {
        kill_children(children_pids, argc - 1);
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }
    
    free(children_pids);
    return 0;
}