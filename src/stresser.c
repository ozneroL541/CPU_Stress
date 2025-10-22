#include "stresser.h"
#include <stdio.h>

THREAD_RETURN mini_stress(void *args) {
    /** Number of blocks in R table */
    unsigned long br = 1024;
    /** Number of blocks in S table */
    unsigned long bs = 1024;
    /** Tables */
    tables * tbls = NULL;
    /** Product parameters */
    product_params * p = NULL;
    /** Thread */
    product_t * t = NULL;
    unsigned char i = 0;

    if ((tbls = init_rs(br, bs)) == NULL){
        #ifdef _WIN32
            return 1;
        #else
            pthread_exit((void *)1);
        #endif
    }
    if ((p = init_product_params(tbls, init_alg_c(original_algorithm), 0)) == NULL) {
        free_tables(tbls, 1);
        #ifdef _WIN32
            return 1;
        #else
            pthread_exit((void *)1);
        #endif
    }

    while (i < 5) {
        t = start_alg(p);
        join_alg(t);
        i++;
    }

    free_product_params(p, 1);

    #ifdef _WIN32
        return (DWORD_PTR) args;
    #else
        pthread_exit(args);
    #endif
}

void stress_test(void) {
    /** Number of CPU threads */
    unsigned short cpu_threads = get_cpu_threads();
    /** Index */
    unsigned long i = 0;
    /** Threads */
    thread_t * threads = NULL;

    /* Allocate threads */
    threads = (thread_t *)malloc(sizeof(thread_t) * cpu_threads);
    if (threads == NULL) {
        return;
    }

    for (i = 0; i < cpu_threads; i++) {
        #ifdef _WIN32
            threads[i] = CreateThread(NULL, 0, mini_stress, NULL, 0, NULL);
        #else
            pthread_create(&threads[i], NULL, mini_stress, NULL);
        #endif
    }
    for (i = 0; i < cpu_threads; i++) {
        #ifdef _WIN32
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]);
        #else
            pthread_join(threads[i], NULL);
        #endif
    }
    free(threads);
}
