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
    *p_high = 0;
    *p_low = 0;

    int ret = 0;

    const char *base_backup = NULL;
    ssize_t nmemb_backup = 0;
    ssize_t p_high_backup = 0;

    while (nmemb > 0) {
        ssize_t middle_index = nmemb / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if (compared > 0) {
            base = middle + size;
            nmemb = nmemb - 1 - middle_index;
            *p_high += middle_index + 1;
            *p_low += middle_index + 1;
        } else if (compared < 0) {
            nmemb = middle_index;
        } else {
            if (base_backup == NULL) {
                base_backup = base;
                nmemb_backup = nmemb;
                p_high_backup = *p_high;
            }

            ret = 1;
            nmemb = middle_index;
        }
    }
    if (!ret) {
        return ret;
    }
    // Search p_high
    base = base_backup;
    nmemb = nmemb_backup;
    *p_high = p_high_backup;
    while (nmemb > 0) {
        ssize_t middle_index = nmemb / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if (compared == 0) {
            base = middle + size;
            nmemb = nmemb - 1 - middle_index;
            *p_high += middle_index + 1;
        } else {
            nmemb = middle_index;
        }
    }
    return ret;
}