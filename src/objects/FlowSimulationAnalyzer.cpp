#include "FlowSimulationAnalyzer.h"

FlowSimulationAnalyzer::FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallXPos, double rightWallXPos)
:particles{particles}, binNumber{binNumber}, leftWallXPos{leftWallXPos}, rightWallXPos{rightWallXPos}{
    binSize = (rightWallXPos - leftWallXPos) / binNumber;
    densities.resize(binNumber);
    velocities.resize(binNumber);
}

void FlowSimulationAnalyzer::calculateDensitiesAndVelocities(){
    double leftRange = leftWallXPos;
    double rightRange = leftRange + binSize;
    for(int i = 0; i < binNumber; i++){
        for(auto &p : particles){
            if(p.getX()[0] >= leftRange && p.getX()[0] < rightRange){
                densities[i]++;
                velocities[i] += p.getV()[1];
            }
        }
        leftRange = rightRange;
        rightRange += binSize;
    }
    for(int i = 0; i < binNumber; i++){
        velocities[i] /= densities[i];
    }
}

int FlowSimulationAnalyzer::getBinNumber() const { return binNumber; }
double FlowSimulationAnalyzer::getLeftWallXPos() const { return leftWallXPos; }
double FlowSimulationAnalyzer::getRightWallXPos() const { return rightWallXPos; }
double FlowSimulationAnalyzer::getBinSize() const { return binSize; }
const std::vector<double>& FlowSimulationAnalyzer::getDensities() const { return densities; }
const std::vector<double>& FlowSimulationAnalyzer::getVelocities() const { return velocities; }
ParticleContainer &FlowSimulationAnalyzer::getParticles() const { return particles; }