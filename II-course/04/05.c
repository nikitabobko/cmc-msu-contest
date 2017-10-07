#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum 
{
    MAX_GID_NUMBER = 32,
    MAX_LINE_CHARACTERS_COUNT = 1022
};

// returns 1 if arr contains key; otherwise returns 0
int contains(unsigned const *arr, int n, unsigned key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

unsigned char *trim(unsigned char *str) {
    while (isspace(*str)) {
        str++;
    }
    if (*str == '\0') {
        return str;
    }

    unsigned char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }
    end[1] = '\0';

    return str;
}

// returns 1 if have enough rights; otherwise 0
int have_enough_rights(unsigned rights, unsigned requested_rights) {
    return (rights & requested_rights) == requested_rights;
}

int main(void) {
    unsigned uid, rights, gid_len = 0;
    unsigned gid[MAX_GID_NUMBER];
    unsigned char line[MAX_LINE_CHARACTERS_COUNT];

    if (scanf("%u\n", &uid) != 1 || !fgets(line, sizeof(line), stdin) || 
        scanf("%o\n", &rights) != 1) {
        return 1;
    }
    int offset = 0, temp;
    while (sscanf(line + offset, "%u%n", gid + gid_len, &temp) == 1) {
        gid_len++;
        offset += temp;
    }

    unsigned file_uid, file_gid, file_rights;
    while (fgets(line, sizeof(line), stdin)) {
        if (sscanf(line, "%u%u%o%n", &file_uid, &file_gid, &file_rights, &offset) != 3) {
            return 1;
        }
        if (file_uid == uid) {
            file_rights >>= 6;
        } else if (contains(gid, gid_len, file_gid)) {
            file_rights >>= 3;
        }
        file_rights &= 07;

        if (have_enough_rights(file_rights, rights)) {
            printf("%s\n", trim(line + offset));
        }
    }
    return 0;
}