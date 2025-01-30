/**
 * @file FlowSimulationAnalyzer.h
 * @brief Component for analyzing particle density and velocity profiles.
 * @date 2025-01-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include "ParticleContainer.h"
#include <vector>

/// @brief Class modeling and analyzer that computes statistics about the state of a nano-scale flow simulation.
class FlowSimulationAnalyzer {
  private:

    /// @brief A reference to the Particle system.
    ParticleContainer &particles;

    /// @brief The number of bins analyzed.
    int binNumber;

    /// @brief The X coordinate of the left wall of the simulation.
    double leftWallPosX;

    /// @brief The X coordinate of the right wall of the simulation.
    double rightWallPosX;

    /// @brief The size of each bin analyzed.
    double binSize;

    /// @brief The vector corresponding to the number of particles in each bin.
    std::vector<double> densities;

    /// @brief The vector corresponding to the average velocity on the y axis of the particles resided in this bin.
    std::vector<double> velocities;

    /// @brief The number of simulation iterations after which to apply the Analyzer functionality (default: 1000).
    int n_analyzer{1000};

  public:

    /**
     * @brief Initialize the current Analyzer's parameters.
     * 
     * @param particles A reference to the Particle system.
     * @param binNumber The number of bins analyzed.
     * @param leftWallPosX The X coordinate of the left wall of the simulation.
     * @param rightWallPosX The X coordinate of the right wall of the simulation.
     * @param n_analyzer The number of simulation iterations after which to apply the Analyzer functionality (default: 1000).
     */
    FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallPosX, double rightWallPosX,
                           int n_analyzer);

    /**
     * @brief Computes the densities and average velocities of the particles resided in each bin.
     * 
     * @details The function iterates over each bin and looks at each particle in order to modify the density and velocity vectors
     * accordingly.
     * 
     */
    void calculateDensitiesAndVelocities();

    /**
     * @brief The main function that analyzes the simulation.
     * 
     * @param currentstep The current iteration of the simulation, determines whether the Analyzer is applied or not. 
     */
    void analyzeFlow(int currentStep);

    /**
     * @brief Writes the gathered information from the density and velocity vectors into csv files as statistics on the simulation's 
     * state.
     */
    int writeToCSV(std::vector<double> &densities, std::vector<double> &velocities, int fileNumber);

    /// @brief Gets the bin number of the analyzer.
    int getBinNumber() const;

    /// @brief Gets the X coordinate of the left wall of the simulation.
    double getLeftWallPosX() const;

    /// @brief Gets the X coordinate of the right wall of the simulation. 
    double getRightWallPosX() const;

    /// @brief Gets the size of the bins used in the computation of statistics. 
    double getBinSize() const;

    /// @brief Gets a reference to the Particle system.
    ParticleContainer &getParticles() const;

    /// @brief Gets the vector containing the number of particles in each bin. 
    const std::vector<double> &getDensities() const;

    /// @brief Gets the vector containing the the average velocities of the particles from each bin. 
    const std::vector<double> &getVelocities() const;
};