#include "lib.h"

void umain() {
    u_int sem_id = syscall_init_PV_var(0);
    u_int childId = fork();
    if (childId == 0) {
        syscall_yield();
        int i = 0;
        while (i < 19) {
            syscall_V(sem_id);
            writef("S-val <%d>\n", syscall_check_PV_value(sem_id));
            i++;
            writef("[%x]Prdc %d\n", env->env_id, i);
        }
        i = 0;
        while (i < 2000) {
            syscall_yield();
            i++;
        }
        writef("Env [%x] produced 20, releasing and terminating others...\n");
        syscall_release_PV_var(sem_id);
        user_panic("Should Stop all");
    } else {
        fork();
        fork();
        int j = 0;
        while (j < 20) {
            writef("Try to p\n");
            syscall_P(sem_id);
            int val = syscall_check_PV_value(sem_id);
            if (val == -3) {
                user_panic("1111");
            }
            writef("S-val <%d>\n", syscall_check_PV_value(sem_id));
            j++;
            writef("[%x]Cnsm %d\n", env->env_id, j);
        }
    }
}
