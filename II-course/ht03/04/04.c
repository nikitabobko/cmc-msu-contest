/*
Submit a solution for ht03-4-unix/dl/dl-2

Full score: 100
Run penalty:    10
Time limit: 1 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/31 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem ht03-4: unix/dl/dl-2

Напишите программу, которая загружает плагин для генерации случайных чисел, получает из него фабрику классов, создает один генератор случайных чисел и генерирует последовательность случайных чисел заданной длины.

Программе передаются 6 аргументов командной строки:

имя файла, в котором находится плагин
NAME — имя плагина
строка аргументов для конструктора генератора случайных чисел
count — сколько случайных чисел необходимо сгенерировать
a для метода next_int
b для метода next_int
На аргументы наложены следующие ограничения:

NAME — непустая строка
длина имени функции random_NAME_factory не превышает 63 байта
count — целое число на отрезке [0;1000] (концы отрезка включены)
a и b — целые числа, представимые типом int
a ≤ b
Программа должна напечатать на стандартный поток вывода count чисел, полученных с помощью метода next_int у заданного генератора случайных чисел. Каждое число выводите на отдельной строке. Не забывайте выводить \n в конце.

Перед завершением работы все ресурсы должны быть корректно освобождены.

Программа должна проверять все входные параметры и все ошибки, которые могут возникать в процессе ее работы. В случае обнаружения ошибки на стандартный поток ошибок должно быть напечатано разумное сообщение об ошибке, и программа должна завершить выполнения с кодом возврата 1. Аргументы командной строки должны проверяться, даже если они не потребуются при выполнении программы.

Для загрузки плагина в память используйте функции dlopen, dlsym, dlclose.

Все определения интерфейсов доступны в заголовочном файле plugin.h, который вы можете скачать и подключать в вашей программе с помощью

#include "plugin.h"
Этот заголовочный файл будет доступен при компиляции на сервере.

Примечание: тесты 2-32 проверяют корректность обработки ошибок.

Описание интерфейса плагинов

Интерфейс плагина для генерации случайных последовательностей определен следующим образом.

struct RandomGenerator;
struct RandomOperations
{
    void (*destroy)(struct RandomGenerator *self);
    double (*next_double)(struct RandomGenerator *self);
    int (*next_int)(struct RandomGenerator *self, int a, int b);
};
struct RandomGenerator
{
    const struct RandomOperations *ops;
};
В RandomOperations задаются три указателя на функции. Функция destroy освобождает память, занимаемую объектом. Функция next_double возвращает случайное число, равномерно распределенное в интервале [0;1). Функция next_int возвращает целое случайное число, равномерно распределенное на отрезке [a;b] (концы отрезка в него входят).

Функции, вызов которых выполняется по указателю из структуры операций, а первым параметром которым передается указатель на обрабатываемый объект, далее будут называться виртуальными методами.

Указатель self не должен быть NULL, должен быть получен при создании объекта и не указывать на объект, уничтоженный destroy. Если эти условия не выполняются, дальнейшее поведение программы не определено (undefined behavior). Это значит, что код, реализующий функции из RandomOperations может предполагать корректность указателя self и не обязан корректность проверять (в том числе и на NULL).

Создание объектов RandomGenerator выполняется с помощью фабрики классов. Фабрика классов имеет следующий интерфейс:

struct RandomFactory;
struct RandomFactoryOperations
{
    void (*destroy)(struct RandomFactory *self);
    struct RandomGenerator *(*new_instance)(struct RandomFactory *self, const char *args);
};
struct RandomFactory
{
    const struct RandomFactoryOperations *ops;
};
Структура RandomFactoryOperations определяет два виртуальных метода destroy, который уничтожает объект и освобождает память, и new_instance, который возвращает новый объект подкласса RandomGenerator. Аргументы args используются для создания этого нового объекта. Если создать новый объект не удалось, метод new_instance возвращает NULL.

Как и в предыдущем случае, предполагается, что self корректен и не равен NULL. В противном случае поведение не определено.

Фабрика классов полезна, когда необходимо создавать объекты класса, неизвестного на этапе компиляции программы. В нашем случае реализация конкретного генератора случайных чисел идет в комплекте с реализацией фабрики классов для порождения объектов этого генератора случайных чисел. Основная программа некоторым образом получает указатель на фабрику классов, который используется для порождения объектов этого класса.

В простых случаях фабрика классов является синглтоном, то есть существует в работающей программе строго в одном экземпляре. Такая простая фабрика классов может быть реализована следующим образом:

static void factory_destroy(struct RandomFactory *rf)
{
    // nothing to do
}

static struct RandomGenerator *factory_new_instance(struct RandomFactory *rf, const char *args)
{
    // создать новый объект и вернуть указатель на него
}

static const struct RandomFactoryOperations factory_operations =
{
    factory_destroy, factory_new_instance
};

static const struct RandomFactory factory =
{
    &factory_operations
};
То есть такая фабрика классов объявлена как константная структура с внутренней (static) видимостью. Тело метода destroy пусто.

Мы будем считать, что каждый плагин содержит ровно один тип генератора случайных чисел, создание объектов которого выполняется с помощью единственной фабрики классов. Тогда чтобы получить фабрику классов нужно вызвать функцию с именем random_NAME_factory, где NAME — имя плагина. Например, если плагин называется my, то получить фабрику классов этого плагина можно следующим фрагментом:

    struct RandomFactory *factory = random_my_factory();
Если по каким-то причинам фабрику классов создать/получить невозможно, функция random_NAME_factory возвращает NULL.

Простейшая функция random_my_factory может быть такой:

struct RandomFactory *random_my_factory(void)
{
    return (struct RandomFactory*) &factory;
}
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <limits.h>
#include "plugin.h"

enum ArgvConsts 
{
    PLUGIN_FILENAME_INDEX = 1,
    PLUGIN_NAME_INDEX = 2,
    RAND_GEN_CONSTRUCTOR_ARGS_INDEX = 3,
    COUNT_INDEX = 4,
    A_INDEX = 5,
    B_INDEX = 6,
};

enum 
{
    GETTING_FACTORY_FUN_NAME_BUF_SIZE = 64,
    NUMERAL_SYSTEM_BASE = 10,
    MAX_COUNT_VALUE = 1000,
};

int arg_str_to_int(const char *str, int *error_occurred) {
    char *endptr;
    errno = 0;
    long num = strtol(str, &endptr, NUMERAL_SYSTEM_BASE);
    if (*endptr != '\0' || *str == '\0' || errno || num > INT_MAX || num < INT_MIN) {
        *error_occurred = 1;
        return -1;
    }
    return num;
}

struct RandomFactory *get_factory(const char *plugin_name, void *handle) {
    char buf[GETTING_FACTORY_FUN_NAME_BUF_SIZE];
    int ret = snprintf(buf, sizeof(buf), "random_%s_factory", plugin_name);
    if (!strcmp(plugin_name, "") || ret >= sizeof(buf) || ret < 0) {
        return NULL;
    }
    
    struct RandomFactory *(*fun)(void) = dlsym(handle, buf);
    if (fun == NULL) {
        return NULL;
    }
    return fun();
}

int main(int argc, char const **argv) {
    int ret_code = 0;
    void *handle = NULL;
    struct RandomFactory *factory = NULL;
    struct RandomGenerator *generator = NULL;

    // Entering try-catch-finally block
    if (argc != 7) {
        ret_code = 1;
        fprintf(stderr, "There should be exact 6 arguments\n");
        goto finally;
    }
    const char *plugin_filename = argv[PLUGIN_FILENAME_INDEX];
    const char *plugin_name = argv[PLUGIN_NAME_INDEX];
    const char *rand_gen_constructor_args = argv[RAND_GEN_CONSTRUCTOR_ARGS_INDEX];

    int error_occurred = 0;
    int a = arg_str_to_int(argv[A_INDEX], &error_occurred);
    int b = arg_str_to_int(argv[B_INDEX], &error_occurred);
    int count = arg_str_to_int(argv[COUNT_INDEX], &error_occurred);
    if (error_occurred || a > b || count > MAX_COUNT_VALUE || count < 0) {
        ret_code = 1;
        fprintf(stderr, "Inappropriate arguments format\n");
        goto finally;
    }

    handle = dlopen(plugin_filename, RTLD_LAZY);
    if (!handle) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while opening plugin\n");
        goto finally;
    }

    factory = get_factory(plugin_name, handle);
    if (!factory) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while getting RandomFactory\n");
        goto finally;
    }
    if (!factory->ops || !factory->ops->new_instance || !factory->ops->destroy) {
        ret_code = 1;
        fprintf(stderr, "Corrupted factory\n");
        goto finally;
    }

    generator = factory->ops->new_instance(factory, rand_gen_constructor_args);
    if (!generator) {
        ret_code = 1;
        fprintf(stderr, "Error occurred while creating new instance of RandomGenerator\n");
        goto finally;
    }
    if (!generator->ops || !generator->ops->next_int || !generator->ops->destroy) {
        ret_code = 1;
        fprintf(stderr, "Corrupted generator\n");
        goto finally;
    }

    for (int i = 0; i < count; ++i) {
        printf("%d\n", generator->ops->next_int(generator, a, b));
    }

finally:
    if (generator && generator->ops && generator->ops->destroy) {
        generator->ops->destroy(generator);
    }
    if (factory && factory->ops && factory->ops->destroy) {
        factory->ops->destroy(factory);
    }
    if (handle) {
        dlclose(handle);
    }
    return ret_code;
}