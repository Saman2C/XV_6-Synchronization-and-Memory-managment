#include "types.h"
#include "stat.h"
#include "user.h"

struct reentrantlock lk;

void f(int n)
{
    acquirereentrantlock(&lk);
    n --;
    if (n)
        f(n);
    releasereentrantlock(&lk);
}

int main(int argc, char *argv[]) {
    initreentrantlock(&lk, "lock");
    f(10);
}