#include "spinlock.h"

struct reentrantlock {
    struct spinlock lock;
    struct proc *owner;
    int recursion;
};
