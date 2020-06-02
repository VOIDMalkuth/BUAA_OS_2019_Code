#include "lib.h"

void umain() {
    u_int sem_id = syscall_init_PV_var(3);
    u_int childId = fork();
    if (childId != 0) {
        int i = 0;
        for (i = 0; i < 10000; i++) {
            syscall_yield();
        }
        i = 0;
        while (i < 19) {
            syscall_V(sem_id);
            writef("S-val <%d>\n", syscall_check_PV_value(sem_id));
            i++;
            writef("[%x]Prdc %d\n", env->env_id, i);
        }
        i = 0;
        while (i < 20000) {
            syscall_yield();
            i++;
        }
        writef("Env [%x] produced 20, releasing and terminating others...\n");
        syscall_release_PV_var(sem_id);
    } else {
        fork();
        fork();
        fork();
        int j = 0;
        while (j < 20) {
            writef("[%x] Try to p, pv val = %d \n", env->env_id, syscall_check_PV_value(sem_id));
            syscall_P(sem_id);
            int val = syscall_check_PV_value(sem_id);
            if (val == -3) {
                user_panic("1111");
            }
            j++;
            writef("[%x]Cnsm %d\n", env->env_id, j);
        }
    }
}
