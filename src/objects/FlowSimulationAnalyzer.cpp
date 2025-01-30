#include "FlowSimulationAnalyzer.h"
#include "io/output/FileWriter.h"
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

/* constructors */
FlowSimulationAnalyzer::FlowSimulationAnalyzer(ParticleContainer &particles) : particles{particles} {
    SPDLOG_TRACE("Created new Analyzer, arguments uninitialized!");
};
FlowSimulationAnalyzer::FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallPosX,
                                               double rightWallPosX, int n_analyzer, const std::string &dirname)
    : particles{particles}, binNumber{binNumber}, leftWallPosX{leftWallPosX}, rightWallPosX{rightWallPosX},
      n_analyzer{n_analyzer}, dirname{dirname} {
    SPDLOG_TRACE("Created new Analyzer - binNumber: {}, leftWallX: {}, rightWallX: {}, freq: {}, dirname: {}",
                 binNumber, leftWallPosX, rightWallPosX, n_analyzer, dirname);
    binSize = (rightWallPosX - leftWallPosX) / binNumber;
    densities.resize(binNumber);
    velocities.resize(binNumber);
    FileWriter::initializeFolder(dirname);
}

/* functionality */
void FlowSimulationAnalyzer::initialize(int binNumber, double leftWallPosX, double rightWallPosX, int n_analyzer,
                                        const std::string &dirname) {
    this->binNumber = binNumber;
    this->leftWallPosX = leftWallPosX;
    this->rightWallPosX = rightWallPosX;
    this->n_analyzer = n_analyzer;
    this->dirname = dirname;

    SPDLOG_TRACE("Initialized Analyzer - binNumber: {}, leftWallX: {}, rightWallX: {}, freq: {}, dirname: {}",
                 binNumber, leftWallPosX, rightWallPosX, n_analyzer, dirname);

    binSize = (rightWallPosX - leftWallPosX) / binNumber;
    densities.resize(binNumber);
    velocities.resize(binNumber);
    FileWriter::initializeFolder(dirname);
}
void FlowSimulationAnalyzer::calculateDensitiesAndVelocities() {
    double leftRange = leftWallPosX;
    double rightRange = leftRange + binSize;
    for (int i = 0; i < binNumber; i++) {
        for (auto &p : particles) {
            if (p.getX()[0] >= leftRange && p.getX()[0] < rightRange) {
                densities[i]++;
                velocities[i] += p.getV()[1];
            }
        }
        leftRange = rightRange;
        rightRange += binSize;
    }
    for (int i = 0; i < binNumber; i++) {
        if (densities[i] > 0) {
            velocities[i] /= densities[i];
        }
    }
}
void FlowSimulationAnalyzer::analyzeFlow(int currentStep) {
    if (n_analyzer <= 0)
        return;
    if (currentStep % n_analyzer == 0) {
        calculateDensitiesAndVelocities();
        writeToCSV(currentStep);
    }
}
int FlowSimulationAnalyzer::writeToCSV(int fileNumber) {
    // write analysis to csv file
    std::string filePath = dirname + "/" + std::to_string(fileNumber) + ".csv";
    std::ofstream csvFile(filePath);
    if (!csvFile.is_open()) {
        SPDLOG_ERROR("Failed to create or open the file at {}", filePath);
        return EXIT_FAILURE;
    }
    csvFile << "binNr,density,velAvg\n";
    for (int i = 0; i < binNumber; i++) {
        csvFile << i << "," << densities[i] << "," << velocities[i] << "\n";
    }

    // reset densities and velocities to zero
    std::fill(densities.begin(), densities.end(), 0);
    std::fill(velocities.begin(), velocities.end(), 0);

    // cleanup
    SPDLOG_INFO("Wrote analysis to CSV file {}.", filePath);
    csvFile.close();
    return EXIT_SUCCESS;
}
int FlowSimulationAnalyzer::getBinNumber() const { return binNumber; }
double FlowSimulationAnalyzer::getLeftWallPosX() const { return leftWallPosX; }
double FlowSimulationAnalyzer::getRightWallPosX() const { return rightWallPosX; }
double FlowSimulationAnalyzer::getBinSize() const { return binSize; }
int FlowSimulationAnalyzer::getFrequency() const { return n_analyzer; }
const std::string &FlowSimulationAnalyzer::getDirname() const { return dirname; }
const std::vector<double> &FlowSimulationAnalyzer::getDensities() const { return densities; }
const std::vector<double> &FlowSimulationAnalyzer::getVelocities() const { return velocities; }
ParticleContainer &FlowSimulationAnalyzer::getParticles() const { return particles; }