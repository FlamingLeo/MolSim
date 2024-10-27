/**
 * @file Arguments.h
 * @brief Configurable program and simulation arguments.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

/// @brief Enum containing each (valid) type of output writer.
enum class WriterType { VTK, XYZ };

/// @brief Enum containg each possible Simulation to be performed.
enum class SimulationType { VERLET };

/**
 * @brief Struct containing each option configurable via command line arguments.
 */
struct Arguments {
    /// @brief Start time of a simulation (default: 0).
    double startTime{0};
    /// @brief End time of a simulation (default: 1000).
    double endTime{1000};
    /// @brief Duration of a timestep (default: 0.014).
    double delta_t{0.014};
    /// @brief Logging frequency (default: every 10 iterations)
    int itFreq{10};
    /// @brief Output type (default: VTK).
    WriterType type{WriterType::VTK};
    /// @brief Simulation type (default: Verlet).
    SimulationType sim{SimulationType::VERLET};
};
