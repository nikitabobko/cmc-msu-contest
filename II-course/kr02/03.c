/*
No summary
*/
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<string.h>

int main(int argc, char **argv) {
    int i;
    for (i = 1; i < argc;) {
        int j = i;
        while (argv[j] && strcmp(argv[j], "END")) {
            j++;
        }
        argv[j] = NULL;
        pid_t pid = fork();
        if (!pid) {
            execvp(argv[i], argv + i);
            _exit(1);
        } else if (pid < 0) {
            return 1;
        }
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            return 1;
        }
        i = j + 1;
    }
    return 0;
}