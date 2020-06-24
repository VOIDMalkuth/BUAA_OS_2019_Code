#include "lib.h"

int a[100];
int b[100] = {1, 2, 3};
int old = 1111;

void umain()
{
    int c = 15;
    int child = thread_fork();
    if (child == 0) {
        int i = 0;
        for (i = 0; i < 100; i++) {
            syscall_yield();
        }
        writef("Child Got: usersp: 0x%x\n", user_getsp());
        writef("C: a[50] is %d, set to 75\n", a[50]);
        a[50] = 75;
        writef("C: b[3] is %d, set to 18\n", b[3]);
        b[3] = 18;
        writef("C: old is %d, set to 2222\n", old);
        old = 2222;
        writef("C: c is %d, set to 777\n", c);
        c = 777;
    } else {
        writef("Parent Got: usersp: 0x%x\n", user_getsp());
        writef("P: a[50] is %d, set to -10\n", a[50]);
        a[50] = -10;
        writef("P: b[3] is %d, set to -7\n", b[3]);
        b[3] = -7;
        writef("P: old is %d, set to 3333\n", old);
        old = 3333;
        writef("P: c is %d, set to 1080\n", c);
        c = 1080;
    }
    return;
}