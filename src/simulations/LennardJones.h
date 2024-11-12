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
/// Lennard-Jones Force
class LennardJones : public Simulation {
  private:
    /// @brief ParticleContainer containing the particles used in the simulation
    ParticleContainer m_particles;

    /// @brief CuboidGenerator used for initializing the particles from the input file
    CuboidGenerator m_generator;

    /// @brief Constant of depth of potential well
    double m_epsilon;

    /// @brief Constant of zero crossing
    double m_sigma;

    /// @brief Start time of simulation
    double m_startTime;

    /// @brief End time of simulation
    double m_endTime;

    /// @brief Time step between iterations
    double m_delta_t;

    /// @brief Frequency of information logging
    int m_itFreq;

    /// @brief Output type
    WriterType m_type;

    /// @brief Object used for writing information to output files
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating velocity of a particle
    StrategyFactory::VFunc m_calculateV;

    /// @brief Function for calculating position of a particle
    StrategyFactory::XFunc m_calculateX;

    /// @brief Function for calculating force of a particle
    StrategyFactory::FFunc m_calculateF;

  public:

    /** 
     * @brief Constructor for initializing a new LennardJones simulation with given filename and arguments
     * 
     * @param filename Reference to the input file which is passed to the CuboidGenerator constructor
     * @param args Arguments containing relevant simulation parameters
     * @param type Variable used for deciding wether a naive or optimized approach is used for calculating forces of particles (0 or 1)
    */
    LennardJones(const std::string &filename, const Arguments &args, int type = 0);

    /**
     * @brief Constructor for initializing a new LennardJones simulation with given ParticleContainer and arguments
     * 
     * @param pc Reference to the ParticleContainer used for storing particles
     * @param args Arguments containing relevant simulation parameters
     * @param type Variable used for deciding wether a naive or optimized approach is used for calculating forces of particles (0 or 1)
     */
    LennardJones(const ParticleContainer &pc, const Arguments &args, int type = 0);

    /// @brief Default destructor destroying data
    ~LennardJones();

    /// @brief Function used for intializating particles, physics functions and particles
    /// @param type Variable used for deciding wether a naive or optimized approach is used for calculating forces of particles (0 or 1)
    void initializeSimulation(int type = 0);

    /// @brief Function running the simulation
    void runSimulation() override;

    /// @brief Gets the particles used in the simulation
    
    /// @return a reference to the main ParticleContainer
    ParticleContainer &getParticles();
};
