/**
 * @file StrategyFactory.h
 * @brief Factory class for choosing the appropriate functions based on the Simulation.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <functional>
#include <tuple>

/// @brief Class for encapsulating the position and velocity functions.
struct TimeIntegrationFuncs {
    /// @brief Typedef for velocity-calculating functions.
    using VFunc = void (*)(ParticleContainer &, double);
    /// @brief Typedef for position-calculating functions.
    using XFunc = void (*)(ParticleContainer &, double, CellContainer *);
    /// @brief Typedef for velocity-calculating functions for the linked cell method.

    /// @brief The velocity-calculating function.
    VFunc vf;
    /// @brief The position-calculating function.
    XFunc xf;

    /**
     * @brief Constructor for choosing the appropriate functions based on the simulation type.
     *
     * @param type The simulation type.
     * @param lc Whether or not the linked cells method is used.
     */
    explicit TimeIntegrationFuncs(SimulationType type, bool lc);
};

/// @brief Factory class for choosing the appropriate functions based on the Simulation.
class StrategyFactory {
  public:
    /// @brief Typedef for force-calculating functions.
    using FFunc = void (*)(ParticleContainer &, double, CellContainer *);

    /**
     * @brief Return a 2-tuple of the physics functions corresponding to the chosen simulation.
     *
     * @param args The Arguments struct containing the simulation type and linked cells boolean.
     * @param modifier A modifier for which combination of functions get returned (i.e. optimized, non-optimized).
     * @return A 2-tuple of the physics functions corresponding to the chosen simulation.
     */
    static std::tuple<TimeIntegrationFuncs, FFunc> getSimulationFunctions(Arguments &args, int modifier);
};
