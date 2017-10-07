/*
Submit a solution for up04-5-c/texts/unix-access-rights-1

Full score:	100
Run penalty:	10
Time limit:	1 s
Real time limit:	5 s
Memory limit:	64M
Open date:	2017/09/26 10:30:00
Date penalty:	2017/12/31 0-50/14d
Problem up04-5: c/texts/unix-access-rights-1

Первая строка стандартного потока ввода содержит неотрицательное целое число - uid процесса.

Вторая строка содержит список целых неотрицательных чисел (не менее 1 и не более 32) - gid процесса.

Третья строка содержит запрашиваемую операцию (4 - read, 2 - write, 1 - execute), операции могут объединяться с помощью "или", например, 6 - read+write.

Следующие строки содержат описание файлов по одному описанию файла на строку. Описание файла содержит его uid, gid, права доступа - все целые неотрицательные числа, затем имя файла. Все пробельные символы между правами доступа и началом имени файла и перед концом строки не являются частью имени (то есть должны игнорироваться). Права задаются в восьмеричном виде.

На стандартный поток вывода напечатаейте имена файлов, к которым процесс с указанными в начале uid и gid может обращаться с указанной операцией. Каждое имя должно быть напечатано на отдельной строке.

Длины строк на стандартном потоке ввода не превышают 1020 символов.

Методические указания. Для последовательного чтения чисел из строки символов полезен спецификатор %n форматного ввода функции sscanf, либо второй параметр функции strtol. Не нужно вручную разбивать строку на числа.

Продумайте разбиение программы на функции. В частности, выделите проверку принадлежности группы списку групп в отдельную функцию.

Examples

Input
100
200
4
100 200 777 file1
99 200 777 file2
99 199 777 file3
99 199 770 file4
Output
file1
file2
file3

*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum 
{
    MAX_GID_NUMBER = 32,
    MAX_LINE_CHARACTERS_COUNT = 1022
};

// returns 1 if arr contains key; otherwise returns 0
int contains(unsigned const *arr, int n, unsigned key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

unsigned char *trim(unsigned char *str) {
    while (isspace(*str)) {
        str++;
    }
    if (*str == '\0') {
        return str;
    }

    unsigned char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    end[1] = '\0';

    return str;
}

// returns 1 if have enough rights; otherwise 0
int have_enough_rights(unsigned rights, unsigned requested_rights) {
    return (rights & requested_rights) == requested_rights;
}

int main(void) {
    unsigned uid, rights, gid_len = 0;
    unsigned gid[MAX_GID_NUMBER];
    unsigned char line[MAX_LINE_CHARACTERS_COUNT];

    if (scanf("%u\n", &uid) != 1 || !fgets(line, sizeof(line), stdin) || 
        scanf("%o\n", &rights) != 1) {
        return 1;
    }
    int offset = 0, temp;
    while (sscanf(line + offset, "%u%n", gid + gid_len, &temp) == 1) {
        gid_len++;
        offset += temp;
    }

    unsigned file_uid, file_gid, file_rights;
    while (fgets(line, sizeof(line), stdin)) {
        if (sscanf(line, "%u%u%o%n", &file_uid, &file_gid, &file_rights, &offset) != 3) {
            return 1;
        }
        if (file_uid == uid) {
            file_rights >>= 6;
        } else if (contains(gid, gid_len, file_gid)) {
            file_rights >>= 3;
        }
        file_rights &= 07;

        if (have_enough_rights(file_rights, rights)) {
            printf("%s\n", trim(line + offset));
        }
    }
    return 0;
}