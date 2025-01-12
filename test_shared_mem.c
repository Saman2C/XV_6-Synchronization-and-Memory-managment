#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define NCHILD 5 

void test_sharedmem_factorial(int n) {
    int shmid = 0;
    void *addr = (void *)open_shared_mem(shmid); 
    if (addr == (void *)0) {
        printf(1, "Failed to open shared memory\n");
        return;
    }

    *(int *)addr = 1; 

    for (int i = 0; i < NCHILD; i++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "Fork failed\n");
            return;
        } else if (pid == 0) {
            
            for (int j = i + 1; j <= n; j += NCHILD) {
                acquire_lock();; 
                *(int *)addr *= j; 
                release_lock(); ; 
            }
            close_shared_mem(addr); 
            exit();
        }
    }

   
    for (int i = 0; i < NCHILD; i++) {
        wait();
    }

    printf(1, "Factorial of %d: %d\n", n, *(int *)addr);

    close_shared_mem(addr); 
}

int main(void) {
    int n = 5; 
    test_sharedmem_factorial(n);
    exit();
}
