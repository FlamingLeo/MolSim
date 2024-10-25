/**
 * @file Verlet.h
 * @brief Simulation of the Velocity-Störmer-Verlet method.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <list>

/// @brief Class containing the functionality and parameters required to run a
/// simulation of the Velocity-Störmer-Verlet method.
class Verlet : public Simulation {
  private:
    /// @brief The list of particles used in the simulation.
    ParticleContainer m_particles; // TODO ParticleContainer...
    /// @brief The start time of the simulation.
    double m_start_time;
    /// @brief The end time of the simulation.
    double m_end_time;
    /// @brief The timestep used during the simulation.
    double m_delta_t;
    /// @brief The output (logging) frequency for generating a file output.
    int m_it_freq;
    /// @brief The type of the generated output file.
    outputWriter::WriterType m_type;

  public:
    /**
     * @brief Initialize a new Verlet simulation with the given program arguments.
     *
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    Verlet(const Arguments &args);

    /// @brief Default destructor destroying declared data.
    ~Verlet();

    /// @brief Calculates the force for all particles.
    void calculateF();

    /// @brief Calculates the position for all particles.
    void calculateX();

    /// @brief Calculates the velocity for all particles.
    void calculateV();

    /// @brief Runs the simulation.
    void runSimulation() override;

    ParticleContainer &getParticles();
};