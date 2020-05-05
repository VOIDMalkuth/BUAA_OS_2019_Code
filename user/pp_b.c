#include "lib.h"

#define AENVID (0x800)
#define BENVID (0x1001)
#define CENVID (0x1802)

void
umain(void)
{
    int r;
    int aNum = -1;
    int bNum = -1;
    u_int sender = 0;
    int tmpNum = 0;
    
    // start
    while (aNum < 5 || bNum < 5) {
        writef("\n@@@@@send %d from %x to %x and %x\n", aNum + 1, syscall_getenvid(), AENVID, CENVID);
        r = ipc_send_double(AENVID, CENVID, aNum + 1, 0, 0);
        writef("%x am waiting.....\n", syscall_getenvid());
        tmpNum = ipc_recv(&sender, 0, 0);
        writef("%x got %d from %x\n", syscall_getenvid(), tmpNum, sender);
        if (sender == AENVID) {
            aNum = tmpNum;
        } else {
            bNum = tmpNum;
        }

        writef("%x am waiting.....\n", syscall_getenvid());
        tmpNum = ipc_recv(&sender, 0, 0);
        writef("%x got %d from %x\n", syscall_getenvid(), tmpNum, sender);
        if (sender == AENVID) {
            aNum = tmpNum;
        } else {
            bNum = tmpNum;
        }

        user_assert(aNum == bNum);
    }

    return;
}
