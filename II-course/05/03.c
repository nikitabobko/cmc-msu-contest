/*
Submit a solution for up05-3-unix/time/time-diff-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/03 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up05-3: unix/time/time-diff-1

В аргументе командной строки задается имя текстового файла, который содержит временные отметки. Входной текстовый файл состоит не менее чем из одной строки текста. Каждая строка текста не превышает в длину 1000 символов и содержит одну временную отметку в формате:

YYYY/MM/DD hh:mm:ss
перед YYYY, между DD и hh и после ss может находиться произвольное число пробельных символов, большее 0. В остальном дата записывается точно в указанном формате.

На стандартный поток вывода напечатайте последовательность целых чисел, представляющую собой интервалы в секундах между последовательными временными отметками во входном файле.

Все временные отметки представимы 32-битным типом time_t. Интервалы также представимы знаковыми 32-битными числами.

Учитывайте возможные переходы на летнее и зимнее время!

Examples

Input in input.txt
2011/09/01 08:45:00
2011/09/01 10:20:00
2011/09/01 10:30:00
2011/12/01 23:32:00
2011/11/30 23:32:00
Output
5700
600
7909320
-86400

*/
#include <stdio.h>
#include <time.h>
#include <string.h>

// return 1 if line was read; otherwise 0
int get_next_seconds(FILE *file, time_t *t) {
    struct tm time;
    memset(&time, 0, sizeof(time));
    time.tm_isdst = -1;
    if (fscanf(file, "%d/%d/%d %d:%d:%d", &time.tm_year, &time.tm_mon, 
        &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec) != 6) {
        return 0;
    }
    time.tm_year -= 1900;
    time.tm_mon -= 1;
    *t = mktime(&time);
    return 1;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        return 1;
    }

    time_t last;
    get_next_seconds(file, &last);
    time_t cur;
    while (get_next_seconds(file, &cur)) {
        printf("%li\n", cur - last);
        last = cur;
    }
    return 0;
}