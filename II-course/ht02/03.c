#include <wctype.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL, "");

    wchar_t c;
    int digits = 0, upper = 0, lower = 0;
    while ((c = fgetwc(stdin)) != WEOF) {
        digits += iswdigit(c) != 0;
        upper += iswupper(c) != 0;
        lower += iswlower(c) != 0;
    }
    wprintf(L"%d\n%d\n%d\n", digits, upper, lower);
    return 0;
}