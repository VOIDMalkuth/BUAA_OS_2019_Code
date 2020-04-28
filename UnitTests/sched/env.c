#include "env.h"
#include <stdio.h>

struct Env *envs = NULL;
struct Env *curenv;
struct Env_list env_sched_list[2];

void _env_run(struct Env *e) {
    printf("Env [%d], pri: %d Runs\n", e->env_id, e->env_pri);
    return;
}

u_int mkenvid(struct Env *e)
{
	static u_long next_env_id = 0;

    /*Hint: lower bits of envid hold e's position in the envs array. */
	u_int idx = e - envs;

    /*Hint:  high bits of envid hold an increasing number. */
	return (++next_env_id << (1 + LOG2NENV)) | idx;
}

