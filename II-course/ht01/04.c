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
    char is_p_high_defined = 0;
    while (nmemb > 0) {
        ssize_t middle_index = nmemb / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if (compared > 0) {
            base = middle + size;
            nmemb = nmemb - 1 - middle_index;
            if (!is_p_high_defined) {
                *p_high += middle_index + 1;
            }
            *p_low += middle_index + 1;
            continue;
        }
        if (compared < 0) {
            nmemb = middle_index;
            continue;
        }
        // compared == 0
        if (!is_p_high_defined) {
            ssize_t cur_index = middle_index;
            while (cur_index < nmemb && 
                compar(key, (const char *) base + cur_index * size, user) == 0) {
                cur_index++;
            }
            *p_high += cur_index;
            is_p_high_defined = 1;
        }
        ret = 1;
        nmemb = middle_index;
    }
    return ret;
}