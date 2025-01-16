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
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

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
    std::array<double, 3> domainSize{INFINITY, INFINITY, INFINITY};
    /// @brief Cutoff radius for linked cells (default: 3.0)
    double cutoffRadius{3.0};
    /// @brief The gravity that the particles are exposed to (default: 0).
    double gravity{0.0};
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
    /// @brief The dimensions of the simulation, either 2 or 3 (default: 2).
    size_t dimensions{2};
    /// @brief Bitset containing flags, whether startTime (0), endTime (1), delta_t (2) and basename (3) have been
    /// manually set by the user. If not, use default values depending on the simulation.
    std::bitset<4> argsSet{0b0000};
    /// @brief Returns a string representation of the struct.
    /// @return A string representation of the struct.
    std::string toString() const;

    /**
     * @brief Overload of the equality operator. Checks if two Arguments structs contain the same values.
     *
     * Does not check if argsSet is the same.
     *
     * @param other The Arguments structed to compare the current one with.
     * @return true if both structs contain the same values.
     * @return false if both structs do not contain the same values.
     */
    inline bool operator==(const Arguments &other) const {
        return startTime == other.startTime && endTime == other.endTime && delta_t == other.delta_t &&
               itFreq == other.itFreq && domainSize == other.domainSize && cutoffRadius == other.cutoffRadius &&
               gravity == other.gravity && basename == other.basename && type == other.type && sim == other.sim &&
               linkedCells == other.linkedCells && conditions == other.conditions && dimensions == other.dimensions;
    }
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