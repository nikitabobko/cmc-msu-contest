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