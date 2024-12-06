/**
 * @file Gravity.h
 * @brief Simulation of gravitational orbit using the Velocity-Störmer-Verlet method.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "io/output/WriterFactory.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief Class containing the functionality and parameters required i.a. to run a simulation of Halley's comet using
/// the Velocity-Störmer-Verlet method.
class Gravity : public Simulation {
  public:
    /**
     * @brief Initialize a new Gravity simulation with specific particles and the given program arguments.
     *
     * @param m_particles A ParticleContainer containing the particles used in the simulation.
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    Gravity(ParticleContainer &m_particles, const Arguments &args, int type = 0);

    /**
     * @brief Initialize a new Gravity simulation from a specified file name using the given program arguments.
     *
     * @param filename The path to the file containing the particle information.
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    Gravity(const std::string &filename, const Arguments &args, int type = 0);

    /**
     * @brief Initialize a new Gravity simulation with the given program arguments.
     *
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    explicit Gravity(const Arguments &args, int type = 0);

    /// @brief Default destructor.
    ~Gravity() override;

    /// @brief Runs the simulation.
    void runSimulation() override;
};