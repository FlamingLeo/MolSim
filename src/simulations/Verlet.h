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
#include "io/output/WriterFactory.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
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

    /// @brief A pointer to the FileWriter object used for writing file output.
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating the Particle velocities.
    StrategyFactory::VFunc m_calculateV;

    /// @brief Function for calculating the Particle positions.
    StrategyFactory::XFunc m_calculateX;

    /// @brief Function for calculating the force effective on each Particle.
    StrategyFactory::FFunc m_calculateF;

  public:
    /**
     * @brief Initialize a new Verlet simulation with specific particles and the given program arguments.
     *
     * @param m_particles A ParticleContainer containing the particles used in the simulation.
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    Verlet(const ParticleContainer &m_particles, const Arguments &args, int type = 0);

    /**
     * @brief Initialize a new Verlet simulation from a specified file name using the given program arguments.
     *
     * @param filename The path to the file containing the particle information.
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    Verlet(const std::string &filename, const Arguments &args, int type = 0);

    /**
     * @brief Initialize a new Verlet simulation with the given program arguments.
     *
     * @param args An Arguments struct containing relevant simulation parameters.
     * @param type An optional modifier for which combination of functions get returned when the Simulation is
     * initialized (i.e. optimized, non-optimized).
     */
    explicit Verlet(const Arguments &args, int type = 0);

    /// @brief Default destructor destroying data.
    ~Verlet();

    /**
     * @brief Initializes the writer and the physics functions.
     *
     * @param type A modifier for which combination of functions get returned (i.e. optimized, non-optimized).
     */
    void initializeSimulation(int type = 0);

    /// @brief Runs the simulation.
    void runSimulation() override;

    /// @brief Gets a reference to the ParticleContainer storing the simulation's particles.
    /// @return A reference to the simulation's ParticleContainer.
    ParticleContainer &getParticles();
};