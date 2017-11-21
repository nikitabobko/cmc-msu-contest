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
    LOCK_ONE_INDEX = 0,
    LOCK_TWO_INDEX = 1,
    NUM_INDEX = 2,
};

void play_ping_pong(int n_times, int child, volatile int *own_lock_mem, 
        volatile int *other_child_lock_mem, volatile int *num_mem) {
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
    volatile int *mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        return 0;
    }
    mem[LOCK_ONE_INDEX] = 0;
    mem[LOCK_TWO_INDEX] = 1;
    mem[NUM_INDEX] = 1;

    errno = 0;
    char *endptr;
    long num = strtol(argv[1], &endptr, NUMERICAL_SYSTEM_BASE);
    if (errno || *endptr != '\0' || num > INT_MAX || num < INT_MIN) {
        return 0;
    }

    pid_t pid = fork();
    if (!pid) {
        play_ping_pong(num, 1, mem + LOCK_ONE_INDEX, mem + LOCK_TWO_INDEX, mem + NUM_INDEX);
    } else if (pid == -1) {
        return 0;
    }
    pid = fork();
    if (!pid) {
        play_ping_pong(num, 2, mem + LOCK_TWO_INDEX, mem + LOCK_ONE_INDEX, mem + NUM_INDEX);
    } else if (pid == -1) {
        return 0;
    }
    wait(NULL);
    wait(NULL);

    return 0;
}