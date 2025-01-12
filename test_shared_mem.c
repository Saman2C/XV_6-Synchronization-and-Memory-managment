#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define NCHILD 5 // Number of child processes

void acquire_user() {
    while ((open("lockfile", O_CREATE | O_WRONLY)) < 0);
}

void release_user() {
    unlink("lockfile");
}

void test_sharedmem_factorial(int n) {
    int shmid = 0;
    void *addr = (void *)open_shared_mem(shmid); // Open shared memory
    if (addr == (void *)-1) {
        printf(1, "Failed to open shared memory\n");
        return;
    }

    *(int *)addr = 1; // Initialize shared memory with factorial(0) = 1

    for (int i = 0; i < NCHILD; i++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "Fork failed\n");
            return;
        } else if (pid == 0) {
            // Child process
            for (int j = i + 1; j <= n; j += NCHILD) {
                acquire_user(); // Acquire lock
                *(int *)addr *= j; // Update factorial value
                release_user(); // Release lock
            }
            close_shared_mem(shmid); // Close shared memory in child
            exit();
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < NCHILD; i++) {
        wait();
    }

    printf(1, "Factorial of %d: %d\n", n, *(int *)addr);

    close_shared_mem(shmid); // Close shared memory in parent
}

int main(void) {
    int n = 5; // Compute factorial of n (change this as needed)
    test_sharedmem_factorial(n);
    exit();
}
