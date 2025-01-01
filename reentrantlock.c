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
    initlock(&(rlock->lock), name);
    rlock->owner = 0;
    rlock->recursion = 0;
}

void
acquirereentrantlock(struct reentrantlock *rlock)
{
    acquire(&rlock->lock);
    if (rlock->owner == myproc())
        rlock->recursion ++;
    else
    {
        while (rlock->owner != 0)
            sleep(rlock, &(rlock->lock));
        rlock->owner = myproc();
        rlock->recursion = 1;
    }
    release(&rlock->lock);
}

void
releasereentrantlock(struct reentrantlock *rlock)
{
    acquire(&rlock->lock);
    if (rlock->owner != myproc())
        panic("releasereentrantlock: not owner\n");
    if (rlock->recursion <= 0)
        panic("releasereentrantlock: not enough recursion\n");
    rlock->recursion --;
    if (rlock->recursion == 0)
    {
        rlock->owner = 0;
        wakeup(rlock);
    }
    release(&rlock->lock);
}

