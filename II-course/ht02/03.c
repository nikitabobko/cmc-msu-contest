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
        }
        if (iswupper(c) != 0) {
            upper++;
        }
        if (iswlower(c) != 0) {
            lower++;
        }
    }
    wprintf(L"%d\n%d\n%d\n", digits, upper, lower);
    return 0;
}