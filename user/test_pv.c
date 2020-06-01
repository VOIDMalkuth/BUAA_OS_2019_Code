#include "lib.h"

void umain() {
    u_int sem_id = syscall_init_PV_var(0);
    u_int childId = fork();
    writef("Semid: %d\n", sem_id);
    if (childId == 0) {
        int i = 0;
        while (i < 30) {
            writef("Semval [%d]\n", syscall_check_PV_value(sem_id));
            i++;
            writef("Prdc %d\n", i);
            syscall_V(sem_id);
        }
    } else {
        int j = 0;
        while (j < 30) {
            syscall_P(sem_id);
            writef("Semval [%d]\n", syscall_check_PV_value(sem_id));
            j++;
            writef("Cnsm %d\n", j);
        }
    }
}
