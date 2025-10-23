#include "alg_threads.h"

THREAD_RETURN run_algorithm(void * params) {
    /** Product thread */
    product_t * p = (product_t *) params;
    p->result = execute_product_simulation(p->params);
    #ifdef _WIN32
        return 0;
    #else
        pthread_exit(NULL);
    #endif
}

product_t * start_alg(product_params* params) {
    /** Product thread */
    product_t * p = (product_t *) malloc(sizeof(product_t));
    if (p != NULL) {
        /** Thread creation result */
        char thread_created = 0;
        p->params = params;
        p->result = 0;
        #ifdef _WIN32
            thread_created = (pthread_create(&(p->thread), NULL, run_algorithm, (void *)p) == 0);
        #else
            thread_created =((p->thread = CreateThread(NULL, 0, run_algorithm, (void *)p, 0, NULL)) != NULL);
        #endif
        if (!thread_created) {
            free(p);
            p = NULL;
        }
    }
    return p;
}

unsigned long join_alg(product_t* p) {
    /** Result */
    unsigned long res = 0;
    if (p != NULL) {
        #ifdef _WIN32
            WaitForSingleObject(p->thread, INFINITE);
            CloseHandle(p->thread);
        #else
            pthread_join(p->thread, NULL);
        #endif
        res = p->result;
        free(p);
    }
    return res;
}

THREAD_RETURN init_t(void * arg) {
    /** Table initialization parameters */
    table_init_params * params = (table_init_params *) arg;
    /** Generated table */
    params->table = gen_table(params->size);
    #ifdef _WIN32
        return 0;
    #else
        pthread_exit(NULL);
    #endif
}

tables * init_rs(const unsigned long r_blocks, const unsigned long s_blocks) {
    /** Table initializer thread */
    thread_t r_t, s_t;
    /** Tables */
    char * r = NULL, * s = NULL;
    /** Tables structure */
    tables * tbl = NULL;
    /** R table initialization parameters */
    table_init_params * r_params = NULL;
    /** S table initialization parameters */
    table_init_params * s_params = NULL;

    if ((r_params = (table_init_params *) malloc(sizeof(table_init_params))) == NULL) {
        return NULL;
    }
    if ((s_params = (table_init_params *) malloc(sizeof(table_init_params))) == NULL) {
        free(r_params);
        return NULL;
    }
    r_params->size = r_blocks;
    s_params->size = s_blocks;
    r = r_params->table = NULL;
    s = s_params->table = NULL;
    #ifdef _WIN32
        r_t = CreateThread(NULL, 0, init_t, (void *)r_params, 0, NULL);
        s_t = CreateThread(NULL, 0, init_t, (void *)s_params, 0, NULL);
        WaitForSingleObject(r_t, INFINITE);
        WaitForSingleObject(s_t, INFINITE);
        CloseHandle(r_t);
        CloseHandle(s_t);
    #else
        pthread_create(&r_t, NULL, init_t, (void *)r_params);
        pthread_create(&s_t, NULL, init_t, (void *)s_params);
        pthread_join(r_t, NULL);
        pthread_join(s_t, NULL);
    #endif
    r = r_params->table;
    s = s_params->table;
    free(r_params);
    free(s_params);

    if (r == NULL || s == NULL) {
        if (r != NULL) {
            free(r);
        }
        if (s != NULL) {
            free(s);
        }
        return NULL;
    }
    tbl = init_tables(r, s, r_blocks, s_blocks);
    return tbl;
}
