/*
Submit a solution for up01-3-c/strings/mystrspccmp-1

Full score:	100
Run penalty:	10
Time limit:	2 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/09/05 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up01-3: c/strings/mystrspccmp-1

Напишите функцию

int mystrspccmp(const char *str1, const char *str2);
Функция должна лексикографически сравнивать строки, которые получались бы из строк str1 и str2 полным удалением пробельных символов (whitespace characters). Функция возвращает значение 0, если две строки лексикографически равны, отрицательное значение, если первая строка лексикографически меньше второй, положительное значение, если первая строка лексикографически больше второй.

Гарантируется, что параметры str1 и str2 не равны NULL.

Запрещается использовать дополнительную память для преобразования строк. Запрещается использовать стандартные функции (кроме функций из ctype.h) и операции индексирования. Запрещается использовать операцию *(p + n). Используйте только указатели и указательную арифметику.

Например:

    mystrspccmp("   de", "a") == 1 // допустимо любое положительное значение
*/

#include <ctype.h>

int mystrspccmp(const char *str1, const char *str2) 
{
    while (1) {
        while (isspace(*str1)) {
            str1++;
        }
        while (isspace(*str2)) {
            str2++;
        }
        if (*str1 != *str2 || *str1 == '\0') {
            break;
        }
        str1++;
        str2++;
    }
    return ((unsigned const char)*str1) - ((unsigned const char)*str2);
}