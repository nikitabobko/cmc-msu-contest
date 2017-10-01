#include <stdio.h>
#include <sys/types.h>

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
    char search_p_high_mode = 0;
    char is_p_high_defined = 0;

    const char *base_backup = NULL;
    ssize_t nmemb_backup = -1;
    ssize_t p_high_backup = NULL;
    while (nmemb > 0 || !is_p_high_defined) {
        if (nmemb == 0 && !is_p_high_defined) {
            // already in search mode and p_high was found
            if (search_p_high_mode) {
                return 1;
            }
            if (base_backup == NULL) {
                return 0;
            }
            base = base_backup;
            nmemb = nmemb_backup;
            *p_high = p_high_backup;
            search_p_high_mode = 1;
        }
        ssize_t middle_index = nmemb / 2;
        const char *middle = (const char *) base + middle_index * size;
        int compared = compar(key, middle, user);
        if ((!search_p_high_mode && compared > 0) || (search_p_high_mode && compared == 0)) {
            base = middle + size;
            nmemb = nmemb - 1 - middle_index;
            if (!is_p_high_defined) {
                *p_high += middle_index + 1;
            }
            if (!search_p_high_mode) {
                *p_low += middle_index + 1;
            }
            continue;
        }
        if ((!search_p_high_mode && compared < 0) || (search_p_high_mode && compared != 0)) {
            nmemb = middle_index;
            continue;
        }
        // go to search p_low
        // but firstly backup current state to be able to find p_high
        if (base_backup == NULL) {
            base_backup = base;
            nmemb_backup = nmemb;
            p_high_backup = *p_high;
        }

        ret = 1;
        nmemb = middle_index;
    }
    return ret;
}

int compar(const int *p1, const int *p2, void *user) {
    return *p1 - *p2;
}

int main(int argc, char const *argv[]) {
    int key = 1;
    int arr[] = {1, 1, 3};
    ssize_t p_low, p_high;
    int ret = bsearch2(&key, arr, sizeof(arr)/sizeof(*arr), sizeof(key), (int (*)(const void *, const void *, void *))&compar, NULL, &p_low, &p_high);
    printf("%d %d %d\n", ret, p_low, p_high);
    return 0;
}