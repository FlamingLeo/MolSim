/**
 * @file SimulationLC.h
 * @brief Class for initializing and running a linked-cell simulation.
 * @date 2024-12-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"

/// @brief Class defining a time-integration simulation using the linked cells method.
class SimulationLC : public Simulation {
  private:
    /// @brief The CellContainer used to store and manage cells.
    CellContainer m_cellContainer;

  public:
    /**
     * @brief Constructs a new linked-cell simulation.
     *
     * @param pc The ParticleContainer containing the simulation molecules.
     * @param args The Arguments struct containing the simulation metadata.
     */
    SimulationLC(ParticleContainer &pc, Arguments &args);

    /// @brief Destroys the current SimulationLC object.
    ~SimulationLC();

    /// @brief Main function for running the simulation.
    void runSimulation() override;
};