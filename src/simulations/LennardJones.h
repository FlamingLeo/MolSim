/**
 * @file LennardJones.h
 * @brief Simulation of the Lennard-Jones potential.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "io/output/WriterFactory.h"
#include "objects/CuboidGenerator.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief LennardJones class containing functionality and parameters used to run a simple simulation using the
/// Lennard-Jones Force.
class LennardJones : public Simulation {
  private:
    /// @brief ParticleContainer containing the particles used in the simulation.
    ParticleContainer m_particles;

    /// @brief CuboidGenerator used for initializing the particles from the input file.
    CuboidGenerator m_generator;

    /// @brief The depth of the potential well \f$ \epsilon \f$.
    double m_epsilon;

    /// @brief The constant of zero crossing \f$ \sigma \f$.
    double m_sigma;

    /// @brief The start time \f$ t_0 \f$ of the simulation.
    double m_startTime;

    /// @brief The end time \f$ t_\text{end} \f$ of the simulation.
    double m_endTime;

    /// @brief The timestep \f$ \Delta t \f$ used during the simulation.
    double m_delta_t;

    /// @brief The output (logging) frequency for generating a file output.
    int m_itFreq;

    /// @brief The total number of iterations for which the simulation will run.
    int m_totalIt;

    /// @brief The type of the generated output file.
    WriterType m_type;

    /// @brief A reference to the basename of the generated files.
    const std::string &m_basename;

    /// @brief Object used for writing information to output files.
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating the velocity of a particle.
    StrategyFactory::VFunc m_calculateV;

    /// @brief Function for calculating the position of a particle.
    StrategyFactory::XFunc m_calculateX;

    /// @brief Function for calculating the force of a particle.
    StrategyFactory::FFunc m_calculateF;

  public:
    /**
     * @brief Constructor for initializing a new LennardJones simulation with given filename and arguments
     *
     * @param filename The name of the input file which is passed to the CuboidGenerator constructor.
     * @param args Arguments containing relevant simulation parameters.
     * @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
     * particles.
     */
    LennardJones(const std::string &filename, const Arguments &args, int type = 0);

    /**
     * @brief Constructor for initializing a new LennardJones simulation with given ParticleContainer and arguments.
     *
     * @param pc A reference to the ParticleContainer used for storing particles.
     * @param args Arguments containing relevant simulation parameters.
     * @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
     * particles.
     */
    LennardJones(const ParticleContainer &pc, const Arguments &args, int type = 0);

    /// @brief Default destructor.
    ~LennardJones();

    /// @brief Function used for intializating the particles and physics functions.
    /// @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
    /// particles.
    void initializeSimulation(int type = 0);

    /// @brief Runs the Lennard-Jones simulation.
    void runSimulation() override;

    /// @brief Gets the particles used in the simulation.
    /// @return A reference to the main ParticleContainer.
    ParticleContainer &getParticles();
};
