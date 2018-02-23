/*
Submit a solution for up04-3-c/strings/parse-unix-perms-1

Full score:	100
Run penalty:	10
Time limit:	2 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/09/26 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up04-3: c/strings/parse-unix-perms-1

Напишите функцию

int
parse_rwx_permissions(const char *str);
На вход функции подается строка — права доступа к файлу в формате rwx (9 знаков). Указатель может принимать значение NULL.

Функция должна вернуть соответствующее значение прав доступа, либо -1, если строка не является корректной записью прав доступа. Корректная запись состоит только из символов '-', 'r', 'w', 'x', расположенных в правильном порядке.

Указание: используйте строку-образец "rwxrwxrwx" для разбора входной строки. В функции не должны использоваться константы 3, 8, 9, 10 (даже в виде enum или #define). Используйте битовые операции.

Поскольку строка-образец является константной, ее длину можно вычислить за константное время на этапе компиляции.
*/
#include <stdlib.h>

int parse_rwx_permissions(const char *str) {
    if (str == NULL) {
        return -1;
    }
    unsigned int ret = 0;
    char const pattern[] = "rwxrwxrwx";
    for (int i = 0; i < sizeof(pattern); i++) {
        if ((str[i] != pattern[i] && str[i] != '-') || (pattern[i] == '\0' && str[i] != '\0')) {
            return -1;
        }
        ret <<= 1;
        ret |= str[i] == pattern[i];
    }
    return ret >> 1;
}