/*
No summary
*/
#include<stdio.h>
#include<time.h>
#include<string.h>

int main(void) {
    struct tm time_info;
    unsigned year, mon, week_day;
    scanf("%u %u %u", &year, &mon, &week_day);
    
    int i = 0;
    do {
        i++;
        memset(&time_info, 0, sizeof(time_info));
        time_info.tm_isdst = -1;
        time_info.tm_mon = mon - 1;
        time_info.tm_year = year - 1900;
        time_info.tm_mday = i;
        mktime(&time_info);
    } while (time_info.tm_wday != week_day);
    
    do {
        printf("%d\n", i);
        i += 7;
        memset(&time_info, 0, sizeof(time_info));
        time_info.tm_isdst = -1;
        time_info.tm_mon = mon - 1;
        time_info.tm_year = year - 1900;
        time_info.tm_mday = i;
        mktime(&time_info);
    } while (time_info.tm_mon == mon - 1);
    
    return 0;
}