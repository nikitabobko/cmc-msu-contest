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

void my_exit(int ret_code, char *msg, void *handle_to_close, 
        struct RandomFactory *factory_to_destroy, struct RandomGenerator *generator_to_destroy) {
    if (msg) {
        fprintf(stderr, "%s\n", msg);
    }
    if (generator_to_destroy && generator_to_destroy->ops && 
            generator_to_destroy->ops->destroy) {
        generator_to_destroy->ops->destroy(generator_to_destroy);
    }
    if (factory_to_destroy && factory_to_destroy->ops && factory_to_destroy->ops->destroy) {
        factory_to_destroy->ops->destroy(factory_to_destroy);
    }
    if (handle_to_close) {
        dlclose(handle_to_close);
    }
    exit(ret_code);
}

int arg_str_to_int(const char *str) {
    char *endptr;
    errno = 0;
    long num = strtol(str, &endptr, NUMERAL_SYSTEM_BASE);
    if (*endptr != '\0' || *str == '\0' || errno || num > INT_MAX || num < INT_MIN) {
        my_exit(1, "Inappropriate arguments format", NULL, NULL, NULL);
    }
    return num;
}

struct RandomFactory *get_factory(const char *plugin_name, void *handle) {
    char buf[GETTING_FACTORY_FUN_NAME_BUF_SIZE];
    int ret = snprintf(buf, sizeof(buf), "random_%s_factory", plugin_name);
    if (!strcmp(plugin_name, "")) {
        my_exit(1, "Empty plugin name", handle, NULL, NULL);
    }
    if (ret >= sizeof(buf) || ret < 0) {
        my_exit(1, "Too long plugin name", handle, NULL, NULL);
    }
    
    struct RandomFactory *(*fun)(void) = dlsym(handle, buf);
    if (fun == NULL) {
        my_exit(1, "Error occurred while getting function of getting factory from "
                "plugin", handle, NULL, NULL);
    }
    return fun();
}

int main(int argc, char const **argv) {
    if (argc != 7) {
        my_exit(1, "There should be exact 6 arguments", NULL, NULL, NULL);
    }
    const char *plugin_filename = argv[PLUGIN_FILENAME_INDEX];
    const char *plugin_name = argv[PLUGIN_NAME_INDEX];
    const char *rand_gen_constructor_args = argv[RAND_GEN_CONSTRUCTOR_ARGS_INDEX];

    int count = arg_str_to_int(argv[COUNT_INDEX]);
    if (count > MAX_COUNT_VALUE || count < 0) {
        fprintf(stderr, "count should be larger than zero and less than %d\n", 
                MAX_COUNT_VALUE);
        return 1;
    }
    int a = arg_str_to_int(argv[A_INDEX]);
    int b = arg_str_to_int(argv[B_INDEX]);
    if (a > b) {
        my_exit(1, "a should be less or equal to b", NULL, NULL, NULL);   
    }

    void *handle = dlopen(plugin_filename, RTLD_LAZY);
    if (!handle) {
        my_exit(1, "Error occurred while opening plugin", handle, NULL, NULL);
    }

    struct RandomFactory *factory = get_factory(plugin_name, handle);
    if (!factory) {
        my_exit(1, "Error occurred while getting RandomFactory", handle, factory, NULL);
    }
    if (!factory->ops || !factory->ops->new_instance || !factory->ops->destroy) {
        my_exit(1, "Corrupted factory", handle, factory, NULL);
    }

    struct RandomGenerator *generator = factory->ops->new_instance(factory, 
        rand_gen_constructor_args);
    if (!generator) {
        my_exit(1, "Error occurred while creating new instance of RandomGenerator", handle,
                factory, generator);
    }
    if (!generator->ops || !generator->ops->next_int || !generator->ops->destroy) {
        my_exit(1, "Corrupted generator", handle, factory, generator);
    }

    for (int i = 0; i < count; ++i) {
        printf("%d\n", generator->ops->next_int(generator, a, b));
    }

    my_exit(0, NULL, handle, factory, generator);
}