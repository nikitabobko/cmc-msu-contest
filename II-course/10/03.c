/*
Submit a solution for up10-3-unix/pipe/pipe-3

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    2 s
Memory limit:   64M
Open date:  2017/11/14 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up10-3: unix/pipe/pipe-3

Программе в аргументах командной строки передаются три команды для выполнения, затем два имени файла (всего 5 аргументов командной строки). Программа должна запустить на выполнение процессы, чтобы они выполнялись в конфигурации, соответствующей команде shell:

( cmd1 < file1 && cmd2 ) | cmd3 >> file2
Родительский процесс должен дождаться завершения всех созданных им процессов и сам завершить выполнение.

Считайте, что команды cmd1, cmd2, cmd3 можно запускать на выполнение с помощью execlp.

Запись CMD1 && CMD2 означает, что процесс CMD2 должен быть запущен только после успешного завершения процесса CMD1.

Запись ( CMD ) используется для группировки, при этом вся командная строка CMD выполняется в отдельном процессе, порожденном основным процессом.
*/
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int run_process(const char *cmd, int input_fd, int output_fd, int parallel, pid_t *child_pid) {
    pid_t pid;
    if (!(pid = fork())) {
        if (input_fd != -1) {
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                exit(1);
            }
            close(input_fd);
        }
        if (output_fd != -1) {
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                exit(1);
            }
            close(output_fd);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid < 0) {
        exit(0);
    }
    if (child_pid) {
        *child_pid = pid;
    }
    if (!parallel) {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && !WEXITSTATUS(status);
    } else {
        return 1;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 6) {
        return 0;
    }
    const char *cmd1 = argv[1];
    const char *cmd2 = argv[2];
    const char *cmd3 = argv[3];
    const char *file1_name = argv[4];
    const char *file2_name = argv[5];

    int fd[2];
    if (pipe(fd) < 0) {
        return 0;
    }

    pid_t pid = fork();
    if (!pid) {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
            exit(1);
        }
        close(fd[1]);

        int in = open(file1_name, O_RDONLY);
        if (in < 0) {
            exit(1);
        }

        exit(run_process(cmd1, in, -1, 0, NULL) && run_process(cmd2, -1, -1, 0, NULL));
    } else if (pid < 0) {
        return 0;
    }
    close(fd[1]);

    int out = open(file2_name, O_APPEND | O_CREAT | O_WRONLY, 0664);
    if (out < 0) {
        close(fd[0]);
        wait(NULL);
        return 0;
    }
    pid_t cmd3_pid;
    run_process(cmd3, fd[0], out, 1, &cmd3_pid);
    close(fd[0]);
    close(out);

    wait(NULL);
    wait(NULL);
    return 0;
}