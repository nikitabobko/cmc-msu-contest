#include <stdio.h>
#include <dlfcn.h>

static const char *PATH_TO_LIB = "/lib/libm.so.6";

int main(int argc, const char **argv) {
    if (argc < 2) {
        return 0;
    }

    void *handle = dlopen(PATH_TO_LIB, RTLD_LAZY);
    if (!handle) {
        return 0;
    }

    double (*fun)(double) = dlsym(handle, argv[1]);
    if (!fun) {
        return 0;
    }

    double num;
    while (scanf("%lf", &num) == 1) {
        printf("%.10g\n", fun(num));
    }

    dlclose(handle);
    return 0;
}