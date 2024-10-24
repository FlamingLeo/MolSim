/**
 * @file Simulation.h
 * @brief Interface for Simulation classes.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
class Simulation {
  public:
    // maybe include virtual functions for X,F and V calculations here aswell?
    virtual void runSimulation() = 0;
    virtual ~Simulation() {}
};