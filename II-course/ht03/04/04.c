#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include "plugin.h"

enum ArgvConsts 
{
    INDEX_OF_RAND_GEN_CONSTRUCTOR_ARGS = 3,
    NUM_OF_ARGS_FOLLOWED_AFTER_RAND_GEN_CONSTRUCTOR_ARGS = 3,
    COUNT_RIGHT_OFFSET_IN_ARGV = 2,
    A_RIGHT_OFFSET_IN_ARGV = 1,
    B_RIGHT_OFFSET_IN_ARGV = 0,
};

enum 
{
    GETTING_FACTORY_FUN_NAME_BUF_SIZE = 63,
    NUMERAL_SYSTEM_BASE = 10,
    MAX_COUNT_VALUE = 1000,
};

// first should be NULL or dynamically allocated
char *strcat_dynamically(char *first, int first_len, const char *second, int *result_length) {
    if (first == NULL) {
        *result_length = strlen(second);
        return strdup(second);
    }

    *result_length = first_len + strlen(second);
    errno = 0;
    // Left two bytes free: first for additional space and second for terminator
    char *ptr = realloc(first, *result_length + 2);
    if (ptr == NULL) {
        free(first);
        return NULL;
    }
    first = ptr;
    strcat(first, second);
    return first;
}

int arg_str_to_int(const char *str) {
    char *endptr;
    errno = 0;
    int num = strtol(str, &endptr, NUMERAL_SYSTEM_BASE);
    if (*endptr != '\0') {
        fprintf(stderr, "%s\n", errno == 0 ? "Inappropriate arguments format" : strerror(errno));
        exit(1);
    }
    return num;
}

void my_exit(int ret_code, char *msg, void *handle_to_close, char *rand_gen_constructor_args_to_free, 
        struct RandomFactory *factory_to_destroy, struct RandomGenerator *generator_to_destroy) {
    free(rand_gen_constructor_args_to_free);
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
    if (msg) {
        fprintf(stderr, "%s\n", msg);
    }
    exit(ret_code);
}

struct RandomFactory *get_factory(const char *plugin_name, void *handle, 
        char *rand_gen_constructor_args) {
    char buf[GETTING_FACTORY_FUN_NAME_BUF_SIZE];
    int ret = snprintf(buf, sizeof(buf), "random_%s_factory", plugin_name);
    if (ret >= sizeof(buf) || ret < 0) {
        my_exit(1, "Too long plugin name", handle, rand_gen_constructor_args, NULL, NULL);
    }

    struct RandomFactory *(*fun)(void) = dlsym(handle, buf);
    if (fun == NULL) {
        char *err = dlerror();
        my_exit(1, err == NULL ? "Error occurred while getting function of getting factory from "
                "plugin" : err, handle, rand_gen_constructor_args, NULL, NULL);
    }
    return fun();
}

int main(int argc, char const **argv) {
    if (argc < 6) {
        my_exit(1, "Not enough arguments", NULL, NULL, NULL, NULL);
    }
    const char *plugin_filename = argv[1];
    const char *plugin_name = argv[2];

    int count = arg_str_to_int(argv[argc - 1 - COUNT_RIGHT_OFFSET_IN_ARGV]);
    if (count > MAX_COUNT_VALUE || count < 0) {
        fprintf(stderr, "count should be larger than zero and less than %d\n", 
                MAX_COUNT_VALUE);
        return 1;
    }
    int a = arg_str_to_int(argv[argc - 1 - A_RIGHT_OFFSET_IN_ARGV]);
    int b = arg_str_to_int(argv[argc - 1 - B_RIGHT_OFFSET_IN_ARGV]);
    if (a > b) {
        my_exit(1, "a should be less or equal to b", NULL, NULL, NULL, NULL);   
    }

    char *rand_gen_constructor_args = NULL;
    int rand_gen_constructor_args_len = 0;
    for (int i = INDEX_OF_RAND_GEN_CONSTRUCTOR_ARGS; 
            i <= argc - 1 - NUM_OF_ARGS_FOLLOWED_AFTER_RAND_GEN_CONSTRUCTOR_ARGS; ++i) {
        if (rand_gen_constructor_args != NULL) {
            strcat(rand_gen_constructor_args, " ");
            rand_gen_constructor_args_len++;
        }
        rand_gen_constructor_args = strcat_dynamically(rand_gen_constructor_args, 
                rand_gen_constructor_args_len, argv[i], &rand_gen_constructor_args_len);
        if (!rand_gen_constructor_args) {
            my_exit(1, errno == 0 ? "Error occurred while allocating memory" : strerror(errno), 
                    NULL, NULL, NULL, NULL);       
        }
    }

    void *handle = dlopen(plugin_filename, RTLD_LAZY);
    if (!handle) {
        char *err = dlerror();
        my_exit(1, err == NULL ? "Error occurred while opening plugin" : err, handle, 
                rand_gen_constructor_args, NULL, NULL);
    }

    struct RandomFactory *factory = get_factory(plugin_name, handle, rand_gen_constructor_args);
    if (!factory) {
        my_exit(1, "Error occurred while getting RandomFactory", handle, rand_gen_constructor_args, 
                factory, NULL);
    }
    if (!factory->ops || !factory->ops->new_instance || !factory->ops->destroy) {
        my_exit(1, "Corrupted factory", handle, rand_gen_constructor_args, factory, NULL);
    }

    struct RandomGenerator *generator = factory->ops->new_instance(factory, 
            rand_gen_constructor_args == NULL ? "" : rand_gen_constructor_args);
    if (!generator) {
        my_exit(1, "Error occurred while creating new instance of RandomGenerator", handle, 
                rand_gen_constructor_args, factory, generator);
    }
    if (!generator->ops || !generator->ops->next_int || !generator->ops->destroy) {
        my_exit(1, "Corrupted generator", handle, rand_gen_constructor_args, factory, generator);
    }

    for (int i = 0; i < count; ++i) {
        printf("%d\n", generator->ops->next_int(generator, a, b));
    }

    my_exit(0, NULL, handle, rand_gen_constructor_args, factory, generator);
}