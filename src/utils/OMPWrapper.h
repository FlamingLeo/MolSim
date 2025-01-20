/**
 * @file OMPWrapper.h
 * @brief Helper class for systems that may not have OpenMP functionality.
 * @date 2025-01-19
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifdef _OPENMP
#include <omp.h>
#else
typedef int omp_lock_t;
inline int omp_get_thread_num() { return 0; }
inline int omp_get_num_threads() { return 1; }
inline void omp_init_lock(omp_lock_t *) { return; }
inline void omp_destroy_lock(omp_lock_t *) { return; }
inline void omp_set_lock(omp_lock_t *) { return; }
inline void omp_unset_lock(omp_lock_t *) { return; }
#endif