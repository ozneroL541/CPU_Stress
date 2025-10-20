#include "stresser.h"

THREAD_RETURN mini_stress(void *args) {
    /** Number of blocks in R table */
    unsigned long br = 1024;
    /** Number of blocks in S table */
    unsigned long bs = 1024;
    /** Tables */
    tables * tbls = init_rs(br, bs);
    /** Product parameters */
    product_params * p = init_product_params(tbls, init_alg_c(original_algorithm), 0);
    /** Thread */
    product_t * t = start_alg(p);

    while (1) {
        join_alg(t);
        start_alg(p);
    }

    return args;
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
}
