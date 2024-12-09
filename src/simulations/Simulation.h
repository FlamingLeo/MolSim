/**
 * @file Simulation.h
 * @brief Class for initializing and running a basic simulation.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "io/output/FileWriter.h"
#include "io/output/WriterFactory.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include "utils/Timer.h"
#include <cassert>
#include <memory>
#include <string>

/// @brief Class defining a time-integration simulation.
class Simulation {
  protected:
    /// @brief Reference to the ParticleContainer containing the particles used in the simulation.
    ParticleContainer &m_particles;

    /// @brief Reference to the Arguments struct containing simulation parameters.
    Arguments &m_args;

    /// @brief The total number of iterations for which the simulation will run.
    int m_totalIt;

    /// @brief A pointer to the FileWriter object used for writing file output.
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating the Particle velocities.
    TimeIntegrationFuncs::VFunc m_calculateV;

    /// @brief Function for calculating the Particle positions.
    TimeIntegrationFuncs::XFunc m_calculateX;

    /// @brief Function for calculating the force effective on each Particle.
    StrategyFactory::FFunc m_calculateF;

    /// @brief Timer used to measure simulation runtime and log molecule updates per second (MUPS).
    Timer m_timer;

    /**
     * @brief Base function for initializing Simulation parameters.
     *
     * @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
     * particles.
     */
    void initializeBase(int type);

    /**
     * @brief Runs a basic simulation loop.
     *
     * @details The particle container must not be empty.
     *
     * @param lc An optional parameter for linked cell algorithms.
     */
    void runSimulationLoop(CellContainer *lc = nullptr);

  public:
    /**
     * @brief Constructs a new Simulation.
     *
     * @param pc The ParticleContainer containing the simulation molecules.
     * @param args The Arguments struct containing the simulation metadata.
     */
    Simulation(ParticleContainer &pc, Arguments &args);

    /// @brief Destroys the current Simulation object.
    virtual ~Simulation();

    /// @brief Main function for running the simulation.
    virtual void runSimulation();

    /**
     * @brief Gets a reference to the ParticleContainer storing the simulation's particles.
     *
     * @return A reference to the simulation's ParticleContainer.
     */
    ParticleContainer &getParticles();
};