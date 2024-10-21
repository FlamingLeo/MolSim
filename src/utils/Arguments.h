/**
 * @file Arguments.h
 * @brief Simulation arguments.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

/**
 * @brief Struct containing each option configurable via command line arguments.
 */
struct Arguments
{
    /// @brief Start time of a simulation (default: 0).
    double start_time{0};
    /// @brief End time of a simulation (default: 1000).
    double end_time{1000};
    /// @brief Duration of a timestep (default: 0.014).
    double delta_t{0.014};
    /// @brief Logging frequency (default: every 10 iterations)
    int it_freq{10};
};