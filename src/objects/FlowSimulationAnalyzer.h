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

class FlowSimulationAnalyzer {
  private:
    ParticleContainer &particles;

    int binNumber;

    double leftWallXPos;

    double rightWallXPos;

    double binSize;

    std::vector<double> densities;

    std::vector<double> velocities;

    int n_analyzer;

  public:
    FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallXPos, double rightWallXPos,
                           int n_analyzer);

    void calculateDensitiesAndVelocities();

    void analyzeFlow(int currentStep);

    int writeToCSV(std::vector<double> &densities, std::vector<double> &velocities);

    int getBinNumber() const;

    double getLeftWallXPos() const;

    double getRightWallXPos() const;

    double getBinSize() const;

    ParticleContainer &getParticles() const;

    const std::vector<double> &getDensities() const;

    const std::vector<double> &getVelocities() const;
};