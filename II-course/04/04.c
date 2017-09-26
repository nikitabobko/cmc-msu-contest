void normalize_path(char *buf) {
    char *i;
    for (i = buf; *i != '\0'; i++) {
        char *j = i + 1;
        while (*i == '/' && *j == '/') {
            j++;
        }
        if (j != i + 1) {
            char *k = i + 1;
            while (*j != '\0') {
                *k = *j;
                k++;
                j++;
            }
            *k = '\0';
        }
    }
}