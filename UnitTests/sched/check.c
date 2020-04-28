#include <stdio.h>

#include <pthread.h>

#include "printf.h"
#include "env.h"
#include "sched.h"



#define test() pthread_create(&t, NULL, testSched, NULL)

struct Env envs_orig[1024];

void clearList() {
    LIST_INIT(&env_sched_list[0]);
	LIST_INIT(&env_sched_list[1]);
}

void initEnvs(){
    envs = envs_orig;
    clearList();
    int i = 0;
    for (i = 0; i < 15; i++) {
        envs[i].env_id = mkenvid(&envs[i]);
        envs[i].env_pri = i % 2 + 1;
    }
}

int main(void) {
    pthread_t t = 0;
    void *status;
    int i = 0;

    printf("==== INIT ====\n");
    initEnvs();
    printf("RunEnvTest\n");
    envs[0].env_status = ENV_RUNNABLE;
    env_run(&envs[0]);
    panic("panicTest");
    printf("panic test Finished!\n\n");


    printf("==== env_sched_list[0] have 1 Runnable ====\n");
    clearList();
    printf("--> <Start>\n");
    LIST_INSERT_HEAD(&env_sched_list[0], &envs[1], env_sched_link);
    envs[1].env_status = ENV_RUNNABLE;
    printf("-> Enter\n");
    test();
    pthread_join(t, &status);
    printf("-> Enter again\n");
    test();
    pthread_join(t, status);
    printf("--> <Finished>\n\n");


    printf("==== Empty env_sched_list ====\n");
    clearList();
    printf("--> <Start>\n");
    test();
    pthread_join(t, status);
    printf("--> <Finished>\n\n");


    printf("==== env_sched_list[1] have 1 Runnable ====\n");
    clearList();
    printf("--> <Start>\n");
    LIST_INSERT_HEAD(&env_sched_list[0], &envs[0], env_sched_link);
    envs[0].env_status = ENV_RUNNABLE;
    test();
    pthread_join(t, status);
    printf("--> Not Runnable anymore:\n");
    envs[0].env_status = ENV_NOT_RUNNABLE;
    test();
    sleep(2);
    printf("--> Runnable again!:\n");
    envs[0].env_status = ENV_RUNNABLE;
    pthread_join(t, status);
    printf("--> <Finished>\n\n");

    printf("==== env_sched_list[1] have 1 NOT_Runnable ====\n");
    clearList();
    printf("--> <Start>\n");
    LIST_INSERT_HEAD(&env_sched_list[0], &envs[0], env_sched_link);
    printf("--> Not Runnable:\n");
    envs[0].env_status = ENV_NOT_RUNNABLE;
    test();
    sleep(2);
    printf("--> Runnable again!:\n");
    envs[0].env_status = ENV_RUNNABLE;
    pthread_join(t, status);
    printf("--> <Finished>\n\n");

    printf("==== env_sched_list[0] have 1 and [1] have 2 Runnable ====\n");
    clearList();
    printf("--> <Start>\n");
    LIST_INSERT_HEAD(&env_sched_list[0], &envs[0], env_sched_link);
    envs[0].env_status = ENV_RUNNABLE;
    LIST_INSERT_HEAD(&env_sched_list[1], &envs[1], env_sched_link);
    envs[1].env_status = ENV_RUNNABLE;
    LIST_INSERT_HEAD(&env_sched_list[1], &envs[2], env_sched_link);
    envs[2].env_status = ENV_RUNNABLE;
    for (int i = 0; i < 20; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);
    printf("-> One in [1] NOT_Runnable\n");
    envs[1].env_status = ENV_NOT_RUNNABLE;
    for (int i = 0; i < 15; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);
    printf("-> All in [1] NOT_Runnable\n");
    envs[2].env_status = ENV_NOT_RUNNABLE;
    for (int i = 0; i < 9; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);

    printf("--> All in [0][1] NOT_Runnable\n");
    envs[0].env_status = ENV_NOT_RUNNABLE;
    test();
    sleep(2);
    printf("--> one in [1] Runnable again!:\n");
    envs[2].env_status = ENV_RUNNABLE;
    pthread_join(t, status);
    for (int i = 0; i < 9; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);

    printf("--> one in [0] Runnable again!:\n");
    envs[0].env_status = ENV_RUNNABLE;
    pthread_join(t, status);
    for (int i = 0; i < 15; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);

    printf("--> All Runnable again!:\n");
    envs[1].env_status = ENV_RUNNABLE;
    pthread_join(t, status);
    for (int i = 0; i < 20; i++) {
        test();
        pthread_join(t, &status);
        printf("-> Enter again\n");
    }
    test();
    pthread_join(t, status);
    printf("--> <Finished>\n\n");
    

    return 0;
}