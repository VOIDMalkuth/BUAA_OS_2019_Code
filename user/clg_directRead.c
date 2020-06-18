#include "lib.h"

void
umain(int argc, char **argv)
{
    u_int *addr = (u_int *)0xb3004100;
    writef("Trying to read 0x%x\n", addr);
    writef("Result is [0x%x]\n", *addr);
    return;
}