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

void spin(int *lock_mem) {
    while(*lock_mem);
    // asm(".spin_lock:\n"
    //     "    mov (%0), %%eax\n"
    //     "    test %%eax, %%eax\n"
    //     "    jne .spin_lock\n" : : "g" (lock_mem) : "eax");
}

void lock(int *lock_mem) {
    asm("mov $1, %%eax\n"
        "xchg %%eax, (%0)\n": "+g" (lock_mem) : : "eax");
}

void unlock(int *lock_mem) {
    asm("mov $0, %%eax\n"
        "xchg %%eax, (%0)\n" : "+g" (lock_mem) : : "eax");
}

void play_ping_pong(int n_times, int child, int *own_lock_mem, 
        int *other_child_lock_mem, int *num_mem) {
    if (child == FIRST_LOCKED_CHILD) {
        sched_yield();
        spin(own_lock_mem);
    }
    while (*num_mem <= n_times) {
        printf("%d %d\n", child, (*num_mem)++);

        // fflush(stdout);
        
        lock(own_lock_mem);
        unlock(other_child_lock_mem);
        sched_yield();
        spin(own_lock_mem);
    }
    unlock(other_child_lock_mem);
    exit(0);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return 0;
    }
    // setbuf(stdout, NULL);
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
    wait(NULL);
    wait(NULL);

    return 0;
}