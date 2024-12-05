/**
 * @file LennardJonesLC.h
 * @brief Simulation of the Lennard-Jones potential using the linked-cell algorithm.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "io/output/WriterFactory.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief Linked-cell LennardJones class containing functionality and parameters used to run a simple simulation using
/// the Lennard-Jones Force.
class LennardJonesLC : public Simulation {
  private:
    /// @brief CellContainer used to partition the domain.
    CellContainer m_linkedCells;

    /// @brief The depth of the potential well \f$ \epsilon \f$.
    double m_epsilon;

    /// @brief The constant of zero crossing \f$ \sigma \f$.
    double m_sigma;

  public:
    /**
     * @brief Constructor for initializing a new LennardJones linked cell simulation with given ParticleContainer and
     * arguments.
     *
     * @param pc A reference to the ParticleContainer used for storing particles.
     * @param args Arguments containing relevant simulation parameters.
     * @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
     * particles.
     */
    LennardJonesLC(ParticleContainer &pc, const Arguments &args, int type = 0);

    /// @brief Default destructor.
    ~LennardJonesLC() override;

    /// @brief Runs the Lennard-Jones linked cell simulation.
    void runSimulation() override;

    /**
     * @brief Get a reference to the CellContainer object.
     *
     * @return A reference to the underlying cell container.
     */
    CellContainer &getCellContainer();
};
