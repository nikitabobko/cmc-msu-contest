#include "plugin.h"
#include <stdlib.h>

enum 
{
    MOD = 0x80000000,
    INCREMENT = 12345,
    MULTIPLIER = 1103515245,
};

static int cur = 0;
/* === Generator === */

int next_fun(struct RandomGenerator *rr, int a, int b) {
    return cur++;
}

void destory_fun(struct RandomGenerator *rr) {
    free(rr);
}

static const struct RandomOperations ops_global = {&destory_fun, NULL, &next_fun};

/* === Factory === */

static void factory_destroy(struct RandomFactory *rf) {
    // nothing to do
}

static struct RandomGenerator *factory_new_instance(struct RandomFactory *rf, const char *args) {
    struct RandomGenerator *ptr = calloc(1, sizeof(*ptr));
    if (ptr == NULL) {
        return NULL;
    }
    ptr->ops = &ops_global;
    return ptr;
}

static const struct RandomFactoryOperations factory_operations = {&factory_destroy, 
	                                                              &factory_new_instance};

static const struct RandomFactory factory = {&factory_operations};

struct RandomFactory *random_plugin_factory() {
	return (struct RandomFactory*) &factory;
}