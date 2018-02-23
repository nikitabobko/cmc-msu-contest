/*
Submit a solution for up03-5-unix/files/binary-tree-2

Full score: 100
Run penalty:    10
Input file name:    input.txt
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/09/19 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up03-5: unix/files/binary-tree-2

В аргументах командной строки задается имя входного бинарного файла произвольного доступа. Бинарный файл рассматривается как массив структур:


struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};
Структура описывает вершину бинарного дерева поиска, где key – ключ поиска, а значения left_idx и right_idx – индексы в массиве соответственно левой и правой вершины. Корень дерева находится в элементе массива с индексом 0. Признаком отсутствия соответствующего поддерева в вершине является индекс, равный 0.

Числа в файле хранятся в естественном для x86 представлении (little-endian).

На стандартный поток вывода напечатайте ключи, размещенные в данном дереве, в порядке убывания.

Считывать содержимое всего файла в память целиком одним блоком запрещается. Для чтения содержимого файла необходимо пользоваться низкоуровневым вводом-выводом (open/read/etc). Корректная работа на big-endian архитектуре не требуется. Не используйте низкоуровневые манипуляции с памятью (union, приведение типов указателей). Не используйте библиотечные функции преобразования.

Для вывода ключей можно использовать высокоуровневые потоки (stdout).

Не забывайте выводить '\n' в конце вывода.

В программе должна быть реализована проверка корректности чтения из файла. Минимизируйте число системных вызовов, необходимых для чтения одной записи из файла.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Node;

// returns 0 if all is ok; otherwise returns -1
int
read_node(int fd, Node *node)
{
    ssize_t really_read = 0;
    do {
        if (lseek(fd, -really_read, SEEK_CUR) == -1 || 
            (really_read = read(fd, node, sizeof(*node))) == -1 || really_read == 0) {
            return -1;
        }
    } while(really_read != sizeof(*node));
    return 0;
}

// returns 0 if all is ok; otherwise returns -1
int 
print_tree(int fd, int index) 
{
    Node node;
    if (lseek(fd, index*sizeof(node), SEEK_SET) == -1 || read_node(fd, &node) == -1) {
        return -1;
    }
    if (node.right_idx != 0) {
        print_tree(fd, node.right_idx);
    }
    printf("%d\n", node.key);
    if (node.left_idx != 0) {
        print_tree(fd, node.left_idx);
    }
    return 0;
}

int 
main(int argc, char const *argv[])
{
    if (argc != 2) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    print_tree(fd, 0);
    close(fd);
    return 0;
}
