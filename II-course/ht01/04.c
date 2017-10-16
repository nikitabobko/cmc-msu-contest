/*
Submit a solution for ht01-4-c/algorithms/binary-search-2

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/22 16:00:00
Date penalty:   2017/12/31 0-50/21d
Problem ht01-4: c/algorithms/binary-search-2

Написать функцию bsearch2, реализующую бинарный поиск в массиве произвольных данных.

int
bsearch2(
        const void *key,
        const void *base,
        ssize_t nmemb,
        ssize_t size,
        int (*compar)(const void *p1, const void *p2, void *user),
        void *user,
        ssize_t *p_low,
        ssize_t *p_high);
Параметр key — указатель на значение, разыскиваемое в таблице.

Параметр base — адрес начала массива элементов.

Тип, на который указывает key, должен совпадать с типом элементов массива.

Параметр nmemb — количество элементов в массиве.

Параметр size — размер одного элемента массива.

Параметр compar — функция сравнения разыскиваемого значение и элемента массива. Какой из параметров p1 и p2 является указателем на элемент массива, а какой — key, не определено.

Параметр user — произвольный указатель, без изменения передаваемый в функцию сравнения.

Элементы в массиве base должны быть упорядочены в порядке неубывания, но массив может содержать равные элементы.

В случае, когда искомое значение в массиве отсутствует, функция должна вернуть значение 0 и в переменные, на которые указывают параметры p_low, p_high записать индекс в массиве, на который должно быть вставлено значение, чтобы свойство неубывания значений элементов массива было сохранено.

Например, если ключ поиска меньше элемента в начале массива, в переменные, на которые указывают параметры p_low, p_high, должно быть записано значение 0.

Если ключ поиска больше элемента в конце массива, в переменные, на которые указывают параметры p_low, p_high, должно быть записано значение nmemb.

В случае, когда искомое значение в массиве присутствует, функция должна вернуть значение 1 и в переменные, на которые указывают параметры p_low, p_high, записать минимальный индекс, на котором находится искомое значение, и минимальный индекс, на котором находится значение, большое искомого значения, либо nmemb, если искомое значение находится в конце массива.
*/
int
bsearch2(
        const void *key,
        const void *base,
        ssize_t nmemb,
        ssize_t size,
        int (*compar)(const void *p1, const void *p2, void *user),
        void *user,
        ssize_t *p_low,
        ssize_t *p_high)
{
    if (nmemb == 0) {
        *p_low = 0;
        *p_high = 0;
        return 0;
    }

    *p_high = -1;
    ssize_t p_high_local = nmemb, p_low_local = 0;

    while (p_high_local > p_low_local) {
        ssize_t middle_index = p_low_local + (p_high_local - p_low_local) / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if (compared > 0) {
            p_low_local = middle_index + 1;
        } else if (compared <= 0) {
            if (*p_high == -1 && !compared) {
                *p_high = p_high_local;
            }
            p_high_local = middle_index;
        }
    }
    *p_low = p_low_local;
    if (compar(key, (const char *) base + p_low_local * size, user)) {
        *p_high = p_high_local;
        return 0;
    }
    // Search p_high
    p_high_local = *p_high;
    while (p_high_local > p_low_local) {
        ssize_t middle_index = p_low_local + (p_high_local - p_low_local) / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if (compared == 0) {
            p_low_local = middle_index + 1;
        } else {
            p_high_local = middle_index;   
        }
    }
    *p_high = p_high_local;
    return 1;
}