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

/** 
 * Table thread initializer
 */
THREAD_RETURN init_t(void * arg) {
    /** Size pointer */
    unsigned long * size = (unsigned long *) arg;
    /** Generated table */
    char * table = gen_table(*size);
    #ifdef _WIN32
        return (DWORD_PTR) table;
    #else
        pthread_exit((void *)table);
    #endif
}

tables * init_rs(const unsigned long r_blocks, const unsigned long s_blocks) {
    /** Table initializer thread */
    thread_t r_t, s_t;
    /** Tables */
    char * r = NULL, * s = NULL;
    /** Tables structure */
    tables * tbl = NULL;
    
    #ifdef _WIN32
        r_t = CreateThread(NULL, 0, init_t, (void *)&r_blocks, 0, NULL);
        s_t = CreateThread(NULL, 0, init_t, (void *)&s_blocks, 0, NULL);
        WaitForSingleObject(r_t, INFINITE);
        WaitForSingleObject(s_t, INFINITE);
       {
            /** Result of the thread */
            LPDWORD result = NULL;
            GetExitCodeThread(r_t, result);
            r = (char *)result;
            GetExitCodeThread(s_t, result);
            s = (char *)result;
       }
        CloseHandle(r_t);
        CloseHandle(s_t);
    #else
        pthread_create(&r_t, NULL, init_t, (void *)&r_blocks);
        pthread_create(&s_t, NULL, init_t, (void *)&s_blocks);
        pthread_join(r_t, (void **)&r);
        pthread_join(s_t, (void **)&s);
    #endif

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
