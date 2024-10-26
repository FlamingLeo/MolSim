/**
 * @file Simulation.h
 * @brief Interface for Simulation classes.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

/// @brief Abstract class defining a possible simulation.
class Simulation {
  // maybe include virtual functions for X,F and V calculations here aswell?
  public:
    /// @brief Interface function for running the simulation.
    virtual void runSimulation() = 0;

    /// @brief Destroys the current Simulation object.
    virtual ~Simulation() {}
};