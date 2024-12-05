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
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief LennardJones class containing functionality and parameters used to run a simple simulation using the
/// Lennard-Jones Force.
class LennardJones : public Simulation {
  private:
    /// @brief The depth of the potential well \f$ \epsilon \f$.
    double m_epsilon;

    /// @brief The constant of zero crossing \f$ \sigma \f$.
    double m_sigma;

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
    LennardJones(ParticleContainer &pc, const Arguments &args, int type = 0);

    /// @brief Default destructor.
    ~LennardJones() override;

    /// @brief Runs the Lennard-Jones simulation.
    void runSimulation() override;
};
