/*
Submit a solution for up10-5-unix/pipe/ping-pong-6

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/14 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up10-5: unix/pipe/ping-pong-6

Родитель создает двух сыновей, связывая их двумя каналами. Сыновья начинают обмениваться числами 1, 2, 3, ... Число 1 получает первый сын. При получении числа из входного канала каждый сын печатает на стандартный поток вывода родителя свой номер (1 или 2) и полученное число и пересылает обратно число, на 1 большее. При достижении числа, задаваемого в командной строке, сын не пересылает число в канал, а завершает работу.

Процесс может завершить работу либо при получении максимального числа из канала, либо при наступлении конца файла в канале. Завершать процесс в других случаях не разрешается.

Отец дожидается завершения работы обоих процессов, затем выводит строку Done и завершает работу.

Если в командной строке передано число 5, pid первого процесса - 100, pid второго процесса - 101, то вывод должен быть таким:

1 1
2 2
1 3
2 4
Done
Данные передавать в pipe в текстовом виде, для ввода-вывода использовать высокоуровневый ввод-вывод (printf, scanf и т. д.).

Привязывать файловые дескрипторы к дескрипторам потока можно либо с помощью функции fdopen, либо с помощью перенаправления стандартных потоков ввода-вывода.
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

enum 
{
    BUF_SIZE = 15,
    NUMERICAL_SYSTEM_BASE = 10,
};

void play(FILE *r, FILE *w, int child_num, int last_num) {
    if (w == NULL || r == NULL) {
        exit(1);
    }
    int num;
    while (fscanf(r, "%d", &num) == 1 && num < last_num) {
        printf("%d %d\n", child_num, num);
        fflush(stdout);
        
        fprintf(w, "%d\n", num + 1);
        fflush(w);
    }
    fclose(w);
    exit(0);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 1;
    }
    int to_1[2];
    int to_2[2];
    pipe(to_1);
    pipe(to_2);

    int num = strtol(argv[1], NULL, 10);

    if (!fork()) {
        close(to_2[0]);
        close(to_1[1]);

        play(fdopen(to_1[0], "r"), fdopen(to_2[1], "w"), 1, num);
    }
    if (!fork()) {
        close(to_2[1]);
        close(to_1[0]);

        play(fdopen(to_2[0], "r"), fdopen(to_1[1], "w"), 2, num);
    }
    
    FILE *w = fdopen(to_1[1], "w");
    fprintf(w, "1\n");
    fflush(w);

    fclose(w);    
    close(to_1[0]);
    close(to_2[0]);
    close(to_2[1]);
    
    wait(NULL);
    wait(NULL);
    
    printf("Done\n");
    return 0;
}