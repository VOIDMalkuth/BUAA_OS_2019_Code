#include "lib.h"

#define AENVID (0x800)
#define BENVID (0x1001)
#define CENVID (0x1802)

void
umain(void)
{
    int r;
    int num = 0;
    u_int sender = 0;
    
    // start
    while (num < 5) {
        writef("%x am waiting.....\n", syscall_getenvid());
        num = ipc_recv(&sender, 0, 0);
        writef("%x got %d from %x\n", syscall_getenvid(), num, sender);
        num = num + 1;
        writef("\n@@@@@send %d from %x to %x\n", num, syscall_getenvid(), BENVID);
        ipc_send(BENVID, num, 0, 0);
    }

    return;
}
