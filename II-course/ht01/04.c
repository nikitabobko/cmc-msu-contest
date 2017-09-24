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
    if (nmemb <= 0) {
        *p_high = 0;
        *p_low = 0;
        return 0;
    }
    ssize_t middle_index = nmemb / 2;
    const char *middle = (const char *) base + middle_index * size;
    if (compar(key, middle, user) > 0) {
        int ret = bsearch2(key, middle + size, nmemb - 1 - middle_index, size, compar, user, 
            p_low, p_high);
        *p_low = middle_index + 1 + *p_low;
        *p_high = middle_index + 1 + *p_high;
        return ret;
    }
    if (compar(key, middle, user) < 0) {
        return bsearch2(key, base, middle_index, size, compar, user, p_low, p_high);
    }
    if (compar(key, middle, user) == 0) {
        if (!bsearch2(key, base, middle_index, size, compar, user, p_low, p_high)) {
            *p_low = middle_index;
        }
        ssize_t cur_index = middle_index;
        while (cur_index < nmemb && 
            compar(key, (const char *) base + cur_index * size, user) == 0) {
            cur_index++;
        }
        *p_high = cur_index;
        return 1;
    }
    return 0;
}