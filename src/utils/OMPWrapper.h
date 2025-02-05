/**
 * @file OMPWrapper.h
 * @brief Helper class for systems that may not have OpenMP functionality.
 * @date 2025-01-19
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#if defined(_OPENMP) && _OPENMP < 201811
#define CONTAINER_LOOP(container, it_var) for (auto it_var = (container).begin(); it_var < (container).end(); ++it_var)
#define CONTAINER_REF(it_var) (*it_var)
#else
#define CONTAINER_LOOP(container, it_var) for (auto &it_var : (container))
#define CONTAINER_REF(it_var) (it_var)
#endif

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
inline int omp_get_max_threads() { return 1; }
#endif