/**
 * @file SimulationStrategy.h
 * @brief Strategy (factory?) class for choosing the simulation to be performed.
 * @version 0.1
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Simulation.h"
#include "Verlet.h"
#include "utils/Arguments.h"
#include <memory>

/**
 * @brief Returns a unique pointer to the desired simulation.
 *
 * @param type The simulation to be performed.
 * @param args The program and simulation arguments to be passed to the Simulation constructor.
 * @return A std::unique_ptr<Simulation> instance matching the desired simulation type.
 */
std::unique_ptr<Simulation> createSimulation(SimulationType type, const Arguments& args);