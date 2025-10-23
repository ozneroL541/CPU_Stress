#ifndef _ALG_THREADS_H_
#define _ALG_THREADS_H_

#include "product_executor.h"
#include "tables.h"
#include "threads.h"

/** 
 * Structure to hold parameters and result for the thread
 * @param params Pointer to the product_params structure
 * @param result Result of the algorithm execution
 */
typedef struct {
    /** Pointer to the parameters structure */
    product_params * params;
    /** Result of the algorithm execution */
    unsigned long result;
    /** Thread identifier */
    thread_t thread;
} product_t;

/** 
 * Structure to hold parameters to initialize a table
 * @param size Size of the table to generate
 * @param table Pointer to the generated table
 */
typedef struct {
    /** Size of the table to generate */
    unsigned long size;
    /** Pointer to the generated table */
    char * table;
} table_init_params;

/**
 * Thread function to run the product simulation algorithm
 * @param params Pointer to the parameters for the algorithm
 * @return NULL
 */
THREAD_RETURN run_algorithm(void * params);

/** 
 * Start a new thread to run the algorithm
 * @param params Pointer to the product_params structure
 * @return Pointer to the product_t structure containing thread info
 */
product_t * start_alg(product_params* params);

/** 
 * Join the thread and retrieve the result
 * @param p Pointer to the product_t structure
 * @return Result of the algorithm execution, 0 on error
 */
unsigned long join_alg(product_t* p);

/** 
 * Table thread initializer
 * @param arg Pointer to the table_init_params structure
 * @return NULL
 */
THREAD_RETURN init_t(void * arg);

/** 
 * Initialize the tables structure with generated tables
 * @param r_blocks Number of blocks in the first table
 * @param s_blocks Number of blocks in the second table
 * @return Pointer to the initialized tables structure, NULL on error
 */
tables * init_rs(const unsigned long r_blocks, const unsigned long s_blocks);

#endif
