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
inline int omp_get_thread_num() { return 0; }
inline int omp_get_num_threads() { return 1; }
#endif