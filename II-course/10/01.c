/*
Submit a solution for up10-1-unix/pipe/pipe-5

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/11/14 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up10-1: unix/pipe/pipe-5

Напишите программу. Родитель создает сына, тот - внука, а тот - правнука. Родитель передает в канал текущее время, полученное с помощью системного вызова time в бинарном виде (тип time_t). Сын, внук и правнук считывают время из канала. Сын выводит на экран строку "H:??", где ?? заменяются на текущее количество часов, внук - "M:??", где ?? заменяются на текущее количество минут, правнук - "S:??", где ?? заменяются на текущее количество секунд. Правнук должен вывести число первым, внук - вторым, а сын - третьим. Записывать в канал разрешается только родителю. Родитель дожидается их завершения.

Пример вывода:

S:00
M:10
H:12
*/
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    time_t cur_time = time(NULL);
    int fd[2];
    if (pipe(fd)) {
        return 1;
    }
    write(fd[1], &cur_time, sizeof(cur_time));
    write(fd[1], &cur_time, sizeof(cur_time));
    write(fd[1], &cur_time, sizeof(cur_time));
    close(fd[1]);
    
    if (!fork()) {
        for (int i = 0; i < 3; i++) {
            pid_t pid = (i != 2 ? fork() : 1);
            
            if (pid > 0) {
                time_t raw_time = 0;
                read(fd[0], &raw_time, sizeof(raw_time));
                wait(NULL);

                struct tm timeinfo;
                memset(&timeinfo, 0, sizeof(timeinfo));
                timeinfo.tm_isdst = -1;
                localtime_r(&raw_time, &timeinfo);
                switch(i) {
                    case 0:
                        printf("H:%02d\n", timeinfo.tm_hour);
                        break;
                    case 1:
                        printf("M:%02d\n", timeinfo.tm_min);
                        break;
                    case 2:
                        printf("S:%02d\n", timeinfo.tm_sec);
                        break;    
                    default:
                        break;
                }
                fflush(stdout);
                exit(0);    
            } else if (pid < 0) {
                return 1;
            }    
        }
    }
    wait(NULL);
    
    return 0;
}