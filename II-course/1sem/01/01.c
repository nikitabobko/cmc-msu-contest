/*
Submit a solution for up01-1-c/ints/bitcount-2

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/05 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up01-1: c/ints/bitcount-2

Напишите функцию bitcount со следующим прототипом на Си:

int bitcount(STYPE value);
Где STYPE — это некоторый целый знаковый тип. Кроме того, определен тип UTYPE — это некоторый целый беззнаковый тип того же размера, что и STYPE.

Функция возвращает число единичных бит, установленных в значении параметра value.

Используйте битовые операции для обработки параметра функции.

Отрицательные числа представляются в дополнительном коде.

Не используйте sizeof, не используйте константы, зависящие от размера типов. Не используйте 64-битные типы в процессе вычислений (если только STYPE не 64-битный).

Не используйте GCC __builtin функции.

Для того, чтобы определить типы STYPE и UTYPE в своей программе для тестирования, можно использовать typedef следующим образом:

typedef int STYPE;
typedef unsigned int UTYPE;
Соблюдайте рекомендованный стиль форматирования программ.
*/

int bitcount(STYPE value) {
    UTYPE t = (UTYPE) value;
    int count = 0;
    while (t != 0) {
        count += t & 0x1;
        t >>= 1;
    }
    return count;
}
