#include <stdio.h>

enum 
{
    MAX_GID_NUMBER = 32,
    MAN_LINE_CHARACTERS_COUNT = 1021
};

// returns 1 if arr contains key; otherwise returns 0
int contains(unsigned *arr, int n, unsigned key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

int main() {
    unsigned uid, rights, gid_len = 0;
    unsigned gid[MAX_GID_NUMBER];
    char *line[MAN_LINE_CHARACTERS_COUNT];

    scanf("%u", &uid);
    if (!fgets(line, sizeof(line), stdin)) {
        return 1;
    }
    int offset = 0;
    while (sscanf(line + offset, "%u%n", gid + gid_len, &offset) == 2) {
        gid_len++;
    }
    scanf("%u", &rights);

    

    return 0;
}