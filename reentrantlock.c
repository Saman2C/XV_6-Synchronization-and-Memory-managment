#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "reentrantlock.h"

void
initreentrantlock(struct reentrantlock *rlock, char *name)
{
    struct spinlock lk;
    initlock(&lk, name);
    rlock->lock = lk;
    rlock->owner = 0;
    rlock->recursion = 0;
}

void
acquirereentrantlock(struct reentrantlock *rlock)
{
    struct spinlock *lk = &(rlock->lock);
    acquire(lk);
    if (rlock->owner == myproc())
        rlock->recursion ++;
    else
    {
        while (rlock->owner != 0)
            sleep(rlock, lk);
        rlock->owner = myproc();
        rlock->recursion = 1;
    }
    release(lk);
}

void
releasereentrantlock(struct reentrantlock *rlock)
{
    struct spinlock *lk = &(rlock->lock);
    acquire(lk);
    if (rlock->owner != myproc())
        return;
    rlock->recursion --;
    if (rlock->recursion == 0)
    {
        rlock->owner = 0;
        wakeup(rlock);
    }
    release(lk);
}

