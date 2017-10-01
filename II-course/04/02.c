/*
Submit a solution for up04-2-c/strings/unparse-unix-perms-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/26 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up04-2: c/strings/unparse-unix-perms-1

В аргументах командной строки передаются 9-битные восьмеричные числа, соответствующие правам доступа к файлам в UNIX. Каждый аргумент командной строки содержит только одно число. Незначащие нули у восьмеричного числа могут отсутствовать, а могут и присутствовать. На стандартный поток вывода напечатайте права доступа в rwx-записи. Каждую строку прав доступа выводите на отдельной строке текста.

Для получения строки-результата используйте константную строку-шаблон.

Examples

Input
644 755
Output
rw-r--r--
rwxr-xr-x
*/
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char pattern[] = "rwxrwxrwx";
    for (int i = 1; i < argc; i++) {
        unsigned num;
        sscanf(argv[i], "%o", &num);
        char str[sizeof(pattern)] = {};
        for (int k = 0; k < sizeof(pattern) - 1; k++) {
            str[k] = (num >> (sizeof(pattern) - 2 - k)) & 0x1 ? pattern[k] : '-';
        }
        printf("%s\n", str);
    }
    return 0;
}