#include "FlowSimulationAnalyzer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

FlowSimulationAnalyzer::FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallPosX,
                                               double rightWallPosX, int n_analyzer)
    : particles{particles}, binNumber{binNumber}, leftWallPosX{leftWallPosX}, rightWallPosX{rightWallPosX},
      n_analyzer{n_analyzer} {
    binSize = (rightWallPosX - leftWallPosX) / binNumber;
    densities.resize(binNumber);
    velocities.resize(binNumber);
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
        if(densities[i] > 0){
            velocities[i] /= densities[i];
        }
    }
}

void FlowSimulationAnalyzer::analyzeFlow(int currentStep) {
    if (currentStep % n_analyzer == 0) {
        calculateDensitiesAndVelocities();
        writeToCSV(densities, velocities, currentStep);
    }
}

int FlowSimulationAnalyzer::writeToCSV(std::vector<double> &densities, std::vector<double> &velocities, int fileNumber){

    if(std::filesystem::exists("../../statistics") && fileNumber == 0){
        std::filesystem::remove_all("../../statistics");
        std::filesystem::create_directories("../../statistics");
    }

    if(!(std::filesystem::exists("../../statistics"))){
        std::filesystem::create_directories("../../statistics");
    }

    std::string filePath = "../../statistics/statistics" + std::to_string(fileNumber) + ".csv";

    std::ofstream csvFile(filePath);

    if (!csvFile.is_open()) {
        std::cerr << "Failed to create or open the file at " << filePath << std::endl;
        return EXIT_FAILURE; 
    }

    csvFile << "Bin Number,Density,Velocity Average" << std::endl;

    for(int i = 0; i < binNumber; i++){
        csvFile << i << "," << densities[i] << "," << velocities[i] << std::endl;
    }
    std::fill(densities.begin(), densities.end(), 0);
    std::fill(velocities.begin(), velocities.end(), 0);

    csvFile.close();
    return EXIT_SUCCESS;
}

int FlowSimulationAnalyzer::getBinNumber() const { return binNumber; }
double FlowSimulationAnalyzer::getLeftWallPosX() const { return leftWallPosX; }
double FlowSimulationAnalyzer::getRightWallPosX() const { return rightWallPosX; }
double FlowSimulationAnalyzer::getBinSize() const { return binSize; }
const std::vector<double> &FlowSimulationAnalyzer::getDensities() const { return densities; }
const std::vector<double> &FlowSimulationAnalyzer::getVelocities() const { return velocities; }
ParticleContainer &FlowSimulationAnalyzer::getParticles() const { return particles; }