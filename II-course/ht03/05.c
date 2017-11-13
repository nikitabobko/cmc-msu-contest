#include <sys/mman.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

enum
{
    SHARED_MEM_SIZE = 3 * sizeof(int),
    FIRST_LOCKED_CHILD = 2,
    NUMERICAL_SYSTEM_BASE = 10,
};

void play_ping_pong(int n_times, int child, int *own_lock_mem, 
        int *other_child_lock_mem, int *num_mem) {
    if (child == FIRST_LOCKED_CHILD) {
        // Spin
        while(*own_lock_mem) {
            sched_yield();
        }
    }
    while (*num_mem <= n_times) {
        printf("%d %d\n", child, (*num_mem)++);
        fflush(stdout);

        // Lock itself
        *own_lock_mem = 1;
        // Unlock other child
        *other_child_lock_mem = 0;
        // Spin
        while(*own_lock_mem) {
            sched_yield();
        }
    }
    // Unlock other child
    *other_child_lock_mem = 0;
    exit(0);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 0;
    }
    // Shared memory
    void *mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        return 0;
    }
    int *lock_mem_1 = mem;
    *lock_mem_1 = 0;
    
    int *lock_mem_2 = lock_mem_1 + 1;
    *lock_mem_2 = 1;

    int *num_mem = lock_mem_2 + 1;
    *num_mem = 1;

    errno = 0;
    char *endptr;
    long num = strtol(argv[1], &endptr, NUMERICAL_SYSTEM_BASE);
    if (errno || *endptr != '\0' || num > INT_MAX || num < INT_MIN) {
        return 0;
    }

    pid_t pid = fork();
    if (!pid) {
        play_ping_pong(num, 1, lock_mem_1, lock_mem_2, num_mem);
    } else if (pid == -1) {
        return 0;
    }
    pid = fork();
    if (!pid) {
        play_ping_pong(num, 2, lock_mem_2, lock_mem_1, num_mem);
    } else if (pid == -1) {
        return 0;
    }
    while (wait(NULL) == -1);
    while (wait(NULL) == -1);

    return 0;
}