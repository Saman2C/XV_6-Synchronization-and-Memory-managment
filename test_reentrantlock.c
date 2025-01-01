#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pid = fork();
    test_lock();
    if (pid == 0)
        exit();
    wait();
    exit();
}