#ifndef STRESSER_H
#define STRESSER_H

#include "cpu_specs.h"
#include "alg_threads.h"
#include "original_alg.h"

/**
 * Atomic mini stress function.
 * @param args TMCH
 * @return NULL
 */
THREAD_RETURN mini_stress(void *args);

/** 
 * Stress the CPU by performing intensive computations.
 */
void stress_test(void);

#endif
