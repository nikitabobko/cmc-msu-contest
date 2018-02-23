/*
Submit a solution for ht02-3-c/texts/wchar-stats-1

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   128M
Open date:  2017/10/12 14:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ht02-3: c/texts/wchar-stats-1

На стандартном потоке ввода подается текст. На стандартный поток вывода напечатайте три числа: число цифровых символов, число символов в верхнем регистре и число символов в нижнем регистре.

Выходной поток - текстовый.

Используйте средства работы с Wide Character: тип wchar_t, файл wctype.h.

Examples

Input
Дельта3 Jordan
Output
1
2
10
*/
#include <wctype.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL, "");

    wchar_t c;
    int digits = 0, upper = 0, lower = 0;
    while ((c = fgetwc(stdin)) != WEOF) {
        if (iswdigit(c) != 0) {
            digits++;
        } else if (iswupper(c) != 0) {
            upper++;
        } else if (iswlower(c) != 0) {
            lower++;
        }
    }
    wprintf(L"%d\n%d\n%d\n", digits, upper, lower);
    return 0;
}