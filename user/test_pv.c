#include "lib.h"

void umain() {
    u_int sem_id = syscall_init_PV_var(0);
    u_int childId = fork();
    writef("Semid: %d\n", sem_id);
    if (childId == 0) {
        int i = 0;
        while (i < 30) {
            syscall_V(sem_id);
            writef("Sem val is %d\n", syscall_check_PV_value(sem_id));
            i++;
            writef("Producing %d\n", i);
        }
    } else {
        int j = 0;
        while (j < 30) {
            syscall_P(sem_id);
            writef("Sem val is %d\n", syscall_check_PV_value(sem_id));
            j++;
            writef("Consuming %d\n", j);
        }
    }
}
