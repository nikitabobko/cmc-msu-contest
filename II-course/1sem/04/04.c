/*
Submit a solution for up04-4-c/strings/normalize-path-1

Full score: 100
Run penalty:    10
Time limit: 2 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/26 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up04-4: c/strings/normalize-path-1

Напишите функцию:

void normalize_path(char *buf);
Которая нормализует строку пути, переданную в качестве параметра. В нормализованном пути все подряд идущие символы '/' объединяются в один '/'. Например, ///a/////b должно быть преобразовано в /a/b.

Функция возвращает результат в той же самой области памяти, в которой была получена входная строка.

Запрещается использовать стандартные функции и операцию индексирования. Используйте только указатели и указательную арифметику.
*/
void normalize_path(char *buf) {
    char *i;
    for (i = buf; *i != '\0'; i++) {
        char *j = i + 1;
        while (*i == '/' && *j == '/') {
            j++;
        }
        if (j != i + 1) {
            char *k = i + 1;
            while (*j != '\0') {
                *k = *j;
                k++;
                j++;
            }
            *k = '\0';
        }
    }
}