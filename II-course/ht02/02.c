/*
Submit a solution for ht02-2-c/texts/longest-word-utf-8-1

Full score: 100
Run penalty:    10
Time limit: 2 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/12 14:00:00
Date penalty:   2017/12/31 0-50/14d
Problem ht02-2: c/texts/longest-word-utf-8-1

На стандартный поток ввода подается текст в кодировке UTF-8 (стандарт 2003). Текст представляет собой последовательность слов, состоящих из code points со значением большим U+0020. Слова разделяются произвольным количеством code points со значением не большим U+0020.

В тексте найдите самое длинное слово, где длина слова - это количество составляющих его code points. Если слов максимальной длины несколько, возьмите первое из них. На стандартный поток вывода напечатайте максимальную длину слов в code points, затем, если максимальная длина больше 0, на следующей строке напечатайте само слово в кодировке UTF-8. В конце не забывайте выводить \n.

Если во входном потоке нет слов, максимальная длина полагается равной 0.

Стандартный поток ввода не является файлом произвольного доступа.

Входной текст закодирован в UTF-8 корректно, то есть не содержит code points со значением больше U+10FFFF, в диапазоне [U+D800;U+DFFF], не содержит overlong encodings.

Не делайте никаких предположений о максимальной длине самого длинного слова.

Можете использовать любые средства ввода-вывода. Не допускается использовать средства языка Си для работы с многобайтными кодировками (wide-char functions).

Examples

Input
короткоеслово
длинноеслово
Output
13
короткоеслово
*/
#include <stdio.h>
#include <stdlib.h>

enum 
{
    DEFAULT_CAPACITY = 32,
    MAX_SPACE_CODE_POINT = 0x20
};

int code_point_to_bytes(unsigned char c) {
    if (c >> 7 == 0) {
        return 1;
    } else if (c >> 5 == 0x6) {
        return 2;
    } else if (c >> 4 == 0xe) {
        return 3;
    } else if (c >> 3 == 0x1e) {
        return 4;
    } else if (c >> 2 == 0x3e) {
        return 5;
    } else if (c >> 1 == 0x7e) {
        return 6;
    }
    exit(1);
}

int length(unsigned char *str) {
    if (str == NULL) {
        return 0;
    }
    int len = 0;
    while (*str != '\0') {
        str += code_point_to_bytes(*str);
        len++;
    }
    return len;
}

unsigned char *read_word(void) {
    int c;
    do {
        c = getc_unlocked(stdin);
    } while (c != EOF && c <= MAX_SPACE_CODE_POINT);
    if (c == EOF) {
        return NULL;
    }
    unsigned char *str = NULL;
    int size = 0, pos = 0;
    do {
        if (str == NULL) {
            size = DEFAULT_CAPACITY;
            str = malloc(size * sizeof(*str));
            if (str == NULL) {
                return NULL;
            }
        }
        if (pos + 1 >= size) {
            size *= 2;
            char *ptr = realloc(str, size * sizeof(*str));
            if (ptr == NULL) {
                free(str);
                return NULL;
            }
            str = ptr;
        }
        str[pos++] = (c == '\0' ? MAX_SPACE_CODE_POINT : c);
    } while ((c = getc_unlocked(stdin)) != EOF && c > MAX_SPACE_CODE_POINT);
    str[pos] = '\0';
    return str;
}

int main(void) {
    int max_len = 0, len;
    char *max_len_str = NULL, *str;
    while ((str = read_word())) {
        if ((len = length(str)) > max_len) {
            max_len = len;
            free(max_len_str);
            max_len_str = str;
        } else {
            free(str);
        }
    }

    printf("%d\n", max_len);
    if (max_len > 0) {
        printf("%s\n", max_len_str);
    }

    free(max_len_str);
    return 0;
}