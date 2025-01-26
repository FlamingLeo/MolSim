#pragma once
#include "Simulation.h"

/// @brief Class defining a time-integration simulation using the linked cells method.
class NanoScaleFlowSim : public Simulation {
  private:
    /// @brief The CellContainer used to store and manage cells.
    CellContainer m_cellContainer;

  public:
    /**
     * @brief Constructs a new linked-cell simulation.
     *
     * @param pc The ParticleContainer containing the simulation molecules.
     * @param args The Arguments struct containing the simulation metadata.
     * @param t The Thermostat used for temperature regulation.
     * @param analyzer The Analyzer used for calculating statistics.
     */
    NanoScaleFlowSim(ParticleContainer &pc, Arguments &args, Thermostat &t, FlowSimulationAnalyzer &analyzer);

    /// @brief Destroys the current SimulationLC object.
    ~NanoScaleFlowSim();

    /// @brief Main function for running the simulation.
    void runSimulation() override;
};