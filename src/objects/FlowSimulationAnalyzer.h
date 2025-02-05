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
#include <string>
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

    /// @brief The name of the directory containing the generated .csv files (default: "statistics").
    std::string dirname{"statistics"};

    /**
     * @brief The base name of the generated .csv files (default: "MD_csv").
     *
     * @details During initialization using an XML input, if the substring "vtk" or "xyz" is found in the base name of
     * the output files, it will automatically be replaced with "csv" and used as the base name for the .csv files.
     */
    std::string basename{"MD_csv"};

  public:
    /**
     * @brief Constructs an uninitialized FlowSimulationAnalyzer.
     *
     * @param particles A reference to the Particle system.
     */
    explicit FlowSimulationAnalyzer(ParticleContainer &particles);

    /**
     * @brief Creates and initializes a FlowSimulationAnalyzer.
     *
     * @param particles A reference to the Particle system.
     * @param binNumber The number of bins analyzed.
     * @param leftWallPosX The X coordinate of the left wall of the simulation.
     * @param rightWallPosX The X coordinate of the right wall of the simulation.
     * @param n_analyzer The number of simulation iterations after which to apply the analyzer functionality.
     * @param dirname The name of the directory containing the generated .csv files.
     * @param basename The base name of the generated .csv files.
     */
    FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallPosX, double rightWallPosX,
                           int n_analyzer, const std::string &dirname = "statistics",
                           const std::string &basename = "MD_csv");

    /**
     * @brief Initializes the current object with the given parameters.
     *
     * @param binNumber The number of bins analyzed.
     * @param leftWallPosX The X coordinate of the left wall of the simulation.
     * @param rightWallPosX The X coordinate of the right wall of the simulation.
     * @param n_analyzer The number of simulation iterations after which to apply the analyzer functionality.
     * @param dirname The name of the directory containing the generated .csv files.
     * @param basename The base name of the generated .csv files.
     */
    void initialize(int binNumber, double leftWallPosX, double rightWallPosX, int n_analyzer,
                    const std::string &dirname = "statistics", const std::string &basename = "MD_csv");

    /**
     * @brief Computes the densities and average velocities of the particles resided in each bin.
     *
     * @details The function iterates over each bin and looks at each particle in order to modify the density and
     * velocity vectors accordingly.
     *
     */
    void calculateDensitiesAndVelocities();

    /**
     * @brief The main function that analyzes the simulation.
     *
     * @param currentStep The current iteration of the simulation, determines whether the Analyzer is applied or not.
     */
    void analyzeFlow(int currentStep);

    /**
     * @brief Writes the gathered information from the density and velocity vectors into the corresponding .csv file as
     * statistics based on the simulation's state.
     *
     * @param fileNumber The iteration number to be appended to the filename.
     * @return `EXIT_SUCCESS` if the file was successfully written, `EXIT_FAILURE` otherwise.
     */
    int writeToCSV(int fileNumber);

    /**
     * @brief Gets the bin number of the analyzer.
     *
     * @return The bin number of the analyzer.
     */
    int getBinNumber() const;

    /**
     * @brief Gets the X coordinate of the left wall of the simulation.
     *
     * @return The X coordinate of the left wall of the simulation.
     */
    double getLeftWallPosX() const;

    /**
     * @brief Gets the X coordinate of the right wall of the simulation.
     *
     * @return The X coordinate of the right wall of the simulation.
     */
    double getRightWallPosX() const;

    /**
     * @brief Gets the size of the bins used in the computation of statistics.
     *
     * @return The size of the bins used in the computation of statistics.
     */
    double getBinSize() const;

    /**
     * @brief Gets the output frequency.
     *
     * @return The output frequency.
     */
    int getFrequency() const;

    /**
     * @brief Gets the name of the output directory.
     *
     * @return The name of the output directory.
     */
    const std::string &getDirname() const;

    /**
     * @brief Gets a reference to the analyzed ParticleContainer.
     *
     * @return A reference to the analyzed ParticleContainer.
     */
    ParticleContainer &getParticles() const;

    /**
     * @brief Gets the vector containing the number of particles in each bin (const).
     *
     * @return The const vector containing the number of particles in each bin.
     */
    const std::vector<double> &getDensities() const;

    /**
     * @brief Gets the vector containing the the average velocities of the particles from each bin (const).
     *
     * @return The const vector containing the the average velocities of the particles from each bin.
     */
    const std::vector<double> &getVelocities() const;
};