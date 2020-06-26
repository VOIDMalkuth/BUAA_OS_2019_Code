#include "lib.h"

#define INCLG

void umain() {
    int r = 0, n = 0;
    int *addr = (int *)0x0fffd000;
    syscall_mem_alloc(0, addr, PTE_V | PTE_R | PTE_LIBRARY);
    *addr = 0;

#ifdef INCLG
    int pid = fork();
    if (pid == 0) {
        while (*addr == 0) {
            syscall_yield();
        }
        writef("Child running...\n");
        r = syscall_grant_devperm(pid, 0x1);
        writef("(Child) Granting perm 0x1 to 0x%x, result is %d\n", pid, r);
    } else {
        r = syscall_grant_devperm(pid, 0x7);
        writef("(Parent) Granting perm 0x7 to 0x%x, result is %d\n", pid, r);

        r = syscall_grant_devperm(pid, 0x1);
        writef("(Parent) Granting perm 0x1 to 0x%x, result is %d\n", pid, r);

        r = syscall_grant_devperm(pid, 0x5);
        writef("(Parent) Granting perm 0x5 to 0x%x, result is %d\n", pid, r);
    }
#endif

    writef("NonFS Program Writing RTC\n");
    n = 0x1234;
    r = syscall_write_dev(&n, 0x15000010, 4);
    writef("result is %d, data should be %d\n", r, n);

    writef("NonFS Program Reading RTC\n");
    r = syscall_read_dev(&n, 0x15000010, 4);
    writef("result is %d, data is %d\n", r, n);


    writef("NonFS Program Writing fs\n");
    n = 0x12345678;
    r = syscall_write_dev(&n, 0x13004100, 4);
    writef("result is %d, data should be %d\n", r, n);

    n = 0x0;
    writef("NonFS Program Reading fs\n");
    r = syscall_read_dev(&n, 0x13004100, 4);
    writef("result is %d, data is %d\n", r, n);

#ifdef INCLG
    if (pid != 0) {
        *addr = 1;
        wait(pid);
    }
#endif
    

    return;
}