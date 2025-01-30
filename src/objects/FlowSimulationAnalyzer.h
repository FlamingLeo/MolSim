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

    double leftWallPosX;

    double rightWallPosX;

    double binSize;

    std::vector<double> densities;

    std::vector<double> velocities;

    int n_analyzer;

  public:
    FlowSimulationAnalyzer() = default;

    FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallPosX, double rightWallPosX,
                           int n_analyzer);

    void calculateDensitiesAndVelocities();

    void analyzeFlow(int currentStep);

    int writeToCSV(std::vector<double> &densities, std::vector<double> &velocities, int fileNumber);

    int getBinNumber() const;

    double getLeftWallPosX() const;

    double getRightWallPosX() const;

    double getBinSize() const;

    ParticleContainer &getParticles() const;

    const std::vector<double> &getDensities() const;

    const std::vector<double> &getVelocities() const;
};