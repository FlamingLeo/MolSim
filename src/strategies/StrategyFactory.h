/**
 * @file StrategyChooser.h
 * @brief Factory class for choosing the appropriate functions based on the Simulation.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <functional>
#include <tuple>

/// @brief Factory class for choosing the appropriate functions based on the Simulation.
class StrategyFactory {
  public:
    /// @brief Typedef for velocity-calculating functions.
    using VFunc = void (*)(ParticleContainer &, double);
    /// @brief Typedef for position-calculating functions.
    using XFunc = void (*)(ParticleContainer &, double);
    /// @brief Typedef for force-calculating functions.
    using FFunc = void (*)(ParticleContainer &, double, double);

    /**
     * @brief Return a 3-tuple of the physics functions corresponding to the chosen simulation.
     *
     * @param type The type of the simulation to be performed.
     * @param modifier A modifier for which combination of functions get returned (i.e. optimized, non-optimized).
     * @return A 3-tuple of the physics functions corresponding to the chosen simulation.
     */
    std::tuple<VFunc, XFunc, FFunc> getSimulationFunctions(SimulationType type, int modifier);
};
