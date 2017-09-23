/*
Submit a solution for up01-4-c/files/digit-freq-2

Full score:	100
Run penalty:	10
Time limit:	500 ms
Real time limit:	5 s
Memory limit:	512M
Open date:	2017/09/05 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up01-4: c/files/digit-freq-2

На стандартный поток ввода программе подается текстовый файл, который мог быть подготовлен как в Unix, так и в Windows. Первая строка текста этого файла содержит имя текстового файла для обработки. Последующие строки исходного текстового файла должны игнорироваться.

Если файл существует и может быть прочитан, подсчитайте число вхождений каждого из символов десятичных цифр. Не существующий или нечитаемый файл полагайте пустым. Если стандартный поток ввода пуст, полагайте и обрабатываемый файл пустым. Если длина имени файла для обработки больше 61 полагайте обрабатываемый файл пустым.

Результат выведите в виде таблички:

<символ десятичной цифры> <число вхождений в файл этой цифры>
Не используйте глобальные переменные.

Пример вывода:

0 1
1 12312
2 3
3 0
4 0
5 0
6 0
7 0
8 0
9 0
*/

#include <stdio.h>
#include <ctype.h>

enum 
{
    MAX_NAME_LENGTH = 61,
    NUM_OF_DIGITS = 10,
    FOR_WHITESPACES_AND_TERMINATION = 3
};

void 
print_result(const int *digits) {
    for (int i = 0; i < NUM_OF_DIGITS; i++) {
        printf("%d %d\n", i, digits[i]);
    }
}

int 
main(void)
{
    int arr[NUM_OF_DIGITS] = {};
    char file_name[MAX_NAME_LENGTH + FOR_WHITESPACES_AND_TERMINATION] = {};
    fgets(file_name, sizeof(file_name), stdin);
    int file_name_length = sizeof(file_name);
    for (int i = sizeof(file_name) - 1; i >= 0 && (file_name[i] == '\0' || file_name[i] == '\n' || 
        file_name[i] == '\r'); i--) {
        file_name[i] = '\0';
        file_name_length = i;
    }
    FILE *file;
    if (file_name_length > MAX_NAME_LENGTH || (file = fopen(file_name, "r")) == NULL) {
        print_result(arr);
        return 0;
    }
    int character;
    while ((character = fgetc(file)) != EOF) {
        if (isdigit(character)) {
            arr[character - '0']++;
        }
    }
    print_result(arr);
    fclose(file);
    return 0;
}