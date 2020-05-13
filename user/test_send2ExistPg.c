#include "lib.h"

/* This test could fail with correct vpt entry and wrong data, that's because tlb entry is wrong
 * tlb_invalidate() is not functioning as intended because of curenv is not target env, and the clear will fail
 * */

void umain()
{
    int parentId = syscall_getenvid();
    int childId = fork();

    u_int *va = 0x30000000;     // just pick one

    // both parent and child map page to va
    syscall_mem_alloc(0,va, PTE_R | PTE_V);

    if (childId == 0) {
        va[0] = 0xcccc;
        va[1] = 0x0000;
        va[2] = 0xeeee;
    } else {
        va[0] = 0x1111;
        va[1] = 0x2222;
        va[2] = 0x3333;
    }

    u_int pa = (*vpt)[VPN(va)];
    if (childId == 0) {
        writef("Child: [%x] [%x] [%x] [%x]\n", va[0], va[1], va[2], va[3]);
        writef("ChildVPT Entry: 0x%x\n", pa);
    } else {
        writef("Parent: [%x] [%x] [%x] [%x]\n", va[0], va[1], va[2], va[3]);
        writef("ParentVPT Entry: 0x%x\n", pa);
    }

    if (childId == 0) {
        writef("Child: IPC recving\n");
        int val = ipc_recv(0, va, 0);
        writef("Child: IPC recv fin, val = 0x%x\n", val);
        
        pa = (*vpt)[VPN(va)];
        writef("Child(IPC fin): [%x] [%x] [%x] [%x]\n", va[0], va[1], va[2], va[3]);
        writef("ChildVPT Entry(IPC fin): 0x%x\n", pa);

        // Child and Parent theoretically now have the same data in va
        user_assert(va[0] == 0x1111);
        user_assert(va[1] == 0x2222);
        user_assert(va[2] == 0x3333);
        
        writef("Child: fin\n");
    } else {
        writef("Parent: IPC sending 0x6666 and map [0x%x] to %d\n", va, childId);
        ipc_send(childId, 0x6666, va, PTE_R | PTE_V);
        writef("Parent: IPC send finished\n");
        writef("Parent: fin\n");
    }

    return;
}
