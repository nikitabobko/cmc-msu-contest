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