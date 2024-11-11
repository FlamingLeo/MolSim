/**
 * @file SimulationFactory.h
 * @brief Factory class for choosing the simulation to be performed.
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "LennardJones.h"
#include "Simulation.h"
#include "Verlet.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief Factory class for choosing the simulation to be performed.
class SimulationFactory {
  public:
    /**
     * @brief Returns a unique pointer to the desired simulation.
     *
     * @param type The simulation to be performed.
     * @param filename A string containing the path to the particle data for the simulation.
     * @param args The program and simulation arguments to be passed to the Simulation constructor.
     * @return A std::unique_ptr<Simulation> instance matching the desired simulation type.
     */
    std::unique_ptr<Simulation> createSimulation(SimulationType type, const std::string &filename,
                                                 const Arguments &args);
};