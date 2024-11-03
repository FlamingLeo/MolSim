/**
 * @file Arguments.h
 * @brief Configurable program and simulation arguments.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <iostream>
#include <sstream>
#include <string>

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

    /// @brief Returns a string representation of the struct.
    /// @return A string representation of the struct.
    std::string toString() const;
};

/**
 * @brief Overload of the << operator for passing an Arguments struct to an output stream.
 *
 * @param stream The output stream to be written to.
 * @param args The Arguments struct containing the data itself.
 * @return A reference to the std::ostream for further use.
 */
static inline std::ostream &operator<<(std::ostream &stream, const Arguments &args) {
    return (stream << "{ t_0: " << args.startTime << ", t_end: " << args.endTime << ", dt: " << args.delta_t
                   << ", itFreq: " << args.itFreq << " }");
};

/* toString implementation */
inline std::string Arguments::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}