/**
 * @file SimulationFactory.h
 * @brief Factory class for choosing the simulation to be performed.
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Simulation.h"
#include "SimulationLC.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

/// @brief Factory class for choosing the simulation to be performed.
class SimulationFactory {
  public:
    /**
     * @brief Returns a unique pointer to the desired simulation while passing a ParticleContainer as input.
     *
     * @param pc The ParticleContainer to be used for the simulation.
     * @param args The program and simulation arguments to be passed to the Simulation constructor.
     * @param t The Thermostat used in the simulation.
     * @return A std::unique_ptr<Simulation> instance matching the desired simulation type.
     */
    static std::unique_ptr<Simulation> createSimulation(ParticleContainer &pc, Arguments &args, Thermostat &t);
};