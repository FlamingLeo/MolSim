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
#include <string>

/// @brief Class containing the functionality and parameters required to run a simulation of the Velocity-Störmer-Verlet
/// method.
class Verlet : public Simulation {
  private:
    /// @brief The ParticleContainer containing the particles used in the simulation.
    ParticleContainer m_particles;
    /// @brief The start time \f$ t_0 \f$ of the simulation.
    double m_startTime; // default: 0
    /// @brief The end time \f$ t_\text{end} \f$ of the simulation.
    double m_endTime; // default: 1000
    /// @brief The timestep \f$ \Delta t \f$ used during the simulation.
    double m_delta_t; // default: 0.014
    /// @brief The output (logging) frequency for generating a file output.
    int m_itFreq;
    /// @brief The type of the generated output file.
    WriterType m_type;

  public:
    /**
     * @brief Initialize a new Verlet simulation with specific particles and the given program arguments.
     *
     * @param m_particles A ParticleContainer containing the particles used in the simulation.
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    Verlet(const ParticleContainer &m_particles, const Arguments &args);

    /**
     * @brief Initialize a new Verlet simulation from a specified file name using the given program arguments.
     *
     * @param filename The path to the file containing the particle information.
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    Verlet(const std::string &filename, const Arguments &args);

    /**
     * @brief Initialize a new Verlet simulation with the given program arguments.
     *
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    explicit Verlet(const Arguments &args);

    /// @brief Default destructor destroying data.
    ~Verlet();

    /// @brief Runs the simulation.
    void runSimulation() override;

    /// @brief Gets a reference to the ParticleContainer storing the simulation's particles.
    /// @return A reference to the simulation's ParticleContainer.
    ParticleContainer &getParticles();
};