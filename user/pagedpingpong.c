// Ping-pong a counter between two processes.
// Only need to start one of these -- splits into two with fork.

#include "lib.h"

int name[100] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

volatile int finished = 0;

void
umain(void)
{
	u_int who, i;
    u_int whoami = 0;

	if ((who = fork()) != 0) {
		// get the ball rolling
        whoami = 1;
		writef("\n@@@@@send 0 from %x to %x\n", syscall_getenvid(), who);
        ipc_send(who, 0, name, PTE_V|PTE_R);
		//user_panic("&&&&&&&&&&&&&&&&&&&&&&&&m");
	}

	for (;;) {
		writef("%x am waiting.....\n", syscall_getenvid());
		i = ipc_recv(&who, 0x6000000, 0);

		writef("%x got %d from %x\n", syscall_getenvid(), i, who);

		//user_panic("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
		if (i == 10) {
            name[5] = 10;
            writef("\n");
            writef(whoami?"Child: ":"Parent: ");
            int *a = (int *)0x6000000;
            a[0] = 999;
            int j = 0;
            for (j = 0; j < (BY2PG>>2); j++) {
                writef("%d ", a[j]);
            }
            finished = 1;
			return;
		}

		i++;
		writef("\n@@@@@send %d from %x to %x\n",i, syscall_getenvid(), who);
		ipc_send(who, i, 0, 0);

		if (i == 10) {
            int k = 0;
            while(k++ <= 2000) {
                syscall_yield();
            }
            name[5] = 10;
            writef("\n");
            writef(whoami?"Child: ":"Parent: ");
            int *a = (int *)0x6000000;
            a[0] = 999;
            int j = 0;
            for (j = 0; j < (BY2PG>>2); j++) {
                writef("%d ", a[j]);
            }
			return;
		}
	}

}

