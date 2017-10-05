/*
Submit a solution for up05-1-unix/time/plus-time-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/03 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up05-1: unix/time/plus-time-1

На стандартном потоке ввода записана последовательность целых знаковых 32-битных чисел.

Каждое число обозначает число дней от текущего момента времени.

Для каждого числа на стандартный поток вывода напечатайте календарную дату в локальном часовом поясе, которая будет ровно через это число суток после момента запуска программы.

Календарную дату выводите в формате YYYY-MM-DD, например,

2015-10-12
Если результирующая дата или промежуточные результаты вычислений (число секунд в заданом числе дней) будут непредставимы в типе time_t напечатайте строку OVERFLOW.

Для каждой входной даты ответ выводите на отдельной строке.

Для получения текущего момента времени используйте системный вызов time. Не используйте mktime.


*/
#include <stdio.h>
#include <time.h>

enum
{
    START_YEAR = 1900,
    SECONDS_IN_DAY = 86400
};

int main(int argc, char const *argv[]) {
    time_t day_offset;
    time_t t_cur = time(NULL);
    while (scanf("%li", &day_offset) == 1) {
        if (__builtin_mul_overflow(day_offset, SECONDS_IN_DAY, &day_offset) || 
            __builtin_add_overflow(day_offset, t_cur, &day_offset)) {
            printf("OVERFLOW\n");
        } else {
            struct tm t;
            localtime_r(&day_offset, &t);
            printf("%4d-%02d-%02d\n", t.tm_year + START_YEAR, t.tm_mon + 1, t.tm_mday);    
        }
    }
    return 0;
}