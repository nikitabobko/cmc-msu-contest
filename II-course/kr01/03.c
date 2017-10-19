#include <stdlib.h>
#include <stdio.h>

void my_strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

void normalize_path(char *buf) {
    if (buf == NULL) {
        return;
    }
    char *buf_backup = buf;
    // Always enter this loop when *buf equals '/' 
    while (*buf != '\0') {
        // Truncate buf if contains '/' at the end of path
        if (buf[1] == '\0') {
            // But not truncate if it's root
            if (buf == buf_backup) {
                return;
            }
            *buf = '\0';
            return;
        }
        // Process "/./" or "/.\0" where \0 is terminator
        if (buf[1] == '.' && (buf[2] == '\0' || buf[2] == '/')) {
            my_strcpy(buf + 1, buf + (buf[2] == '\0' ? 2 : 3));
            continue;
        }
        // Process "/../" or "/..\0" where \0 is terminator
        if (buf[1] == '.' && buf[2] == '.' && (buf[3] == '/' || 
                buf[3] == '\0')) {
            char *src = buf + (buf[3] == '\0' ? 3 : 4);
            // Find parent directory but only in case when 
            // we are currently processing not root path
            if (buf != buf_backup) {
                do {
                    buf--;
                } while (*buf != '/');
            }
            my_strcpy(buf + 1, src);
            continue;
        }
        buf++;
        // If it's ordinary folder name then go to the next '/' or terminator in path
        while (*buf != '/' && *buf != '\0') {
            buf++;
        }
    }
}