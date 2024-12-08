/**
 * @file Arguments.h
 * @brief Configurable program and simulation arguments.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "CellUtils.h"
#include <array>
#include <bitset>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#define INF std::numeric_limits<double>::infinity()

/// @brief Enum containing each (valid) type of output writer.
enum class WriterType { VTK, XYZ, NIL };

/// @brief Enum containg each possible Simulation to be performed.
enum class SimulationType { GRAVITY, LJ };

/**
 * @brief Struct containing each option configurable via command line arguments.
 */
struct Arguments {
    /// @brief Start time of a simulation (default: simulation-specific).
    double startTime{};
    /// @brief End time of a simulation (default: simulation-specific).
    double endTime{};
    /// @brief Duration of a timestep (default: simulation-specific).
    double delta_t{};
    /// @brief Logging frequency (default: every 10 iterations)
    int itFreq{10};
    /// @brief Domain size for linked cells (default: unspecified, will fail if not specified!)
    std::array<double, 3> domainSize{INF, INF, INF};
    /// @brief Cutoff radius for linked cells (default: 3.0)
    double cutoffRadius{3.0};
    /// @brief The gravity that the particles are exposed to (default: -12.44).
    double gravity{-12.44};
    /// @brief The basename of the output file (default: type-specific).
    std::string basename{};
    /// @brief Output type (default: VTK).
    WriterType type{WriterType::VTK};
    /// @brief Simulation type (default: LJ).
    SimulationType sim{SimulationType::LJ};
    /// @brief Decide, whether or not to use the linked cell method (default: true)
    bool linkedCells{true};
    /// @brief The type of condition to be applied at each boundary (default: outflow)
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW};
    /// @brief Bitset containing flags, whether startTime (0), endTime (1), delta_t (2) and basename (3) have been
    /// manually set by the user. If not, use default values depending on the simulation.
    std::bitset<4> argsSet{0b0000};
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
}

/* toString implementation */
inline std::string Arguments::toString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}