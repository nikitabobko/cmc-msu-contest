/*
Submit a solution for ku01-3-kr01-3 (upsolving)

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   512M
Open date:  2017/10/17 16:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ku01-3: kr01-3 (upsolving)

Напишите функцию:

void normalize_path(char *buf);
Функция принимает на вход строку абсолютного пути. Строка всегда начинается с символа '/', в строке не встречаются несколько символов '/' подряд.

Если входная строка не равна "/" и оканчивается на '/', то этот символ с конца должен быть удален.

В строке-пути должны быть удалены все вхождения каталогов . и .. исходя из того, что каталог . — это переход на текущий каталог, а каталог .. — это переход на родительский каталог.

Функция модифицирует область памяти, в которой находится входная строка.

Запрещается использовать стандартные функции.

Examples

Input
/a/b/c/
Output
/a/b/c
Input
/a/b/../
Output
/a
Input
/a/b/../../..
Output
/
*/
#include <stdlib.h>
#include <stdio.h>

void my_strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

void normalize_path(char *buf) {
    if (buf == NULL) {
        return;
    }
    char *buf_backup = buf;
    // Always enter this loop when *buf equals '/' 
    while (*buf != '\0') {
        // Truncate buf if contains '/' at the end of path
        if (buf[1] == '\0') {
            // But not truncate if it's root
            if (buf == buf_backup) {
                return;
            }
            *buf = '\0';
            return;
        }
        // Process "/./" or "/.\0" where \0 is terminator
        if (buf[1] == '.' && (buf[2] == '\0' || buf[2] == '/')) {
            my_strcpy(buf + 1, buf + (buf[2] == '\0' ? 2 : 3));
            continue;
        }
        // Process "/../" or "/..\0" where \0 is terminator
        if (buf[1] == '.' && buf[2] == '.' && (buf[3] == '/' || 
                buf[3] == '\0')) {
            char *src = buf + (buf[3] == '\0' ? 3 : 4);
            // Find parent directory but only in case when 
            // we are currently processing not root path
            if (buf != buf_backup) {
                do {
                    buf--;
                } while (*buf != '/');
            }
            my_strcpy(buf + 1, src);
            continue;
        }
        buf++;
        // If it's ordinary folder name then go to the next '/' or terminator in path
        while (*buf != '/' && *buf != '\0') {
            buf++;
        }
    }
}