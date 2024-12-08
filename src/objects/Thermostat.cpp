#include "Thermostat.h"
#include "utils/ArrayUtils.h"

Thermostat::Thermostat(ParticleContainer &particles, int dimension)
:particles{particles}, dimension{dimension}{
}

void Thermostat::calculateKineticEnergy(){
    double sum = 0;
    for(auto &p : particles){
        sum += p.getM() * ArrayUtils::L2Norm(p.getV());
    }
    kineticEnergy = sum/2;
}

void Thermostat::calculateTemp(){
    temperature = 2 * kineticEnergy / (dimension * particles.activeSize());
}

void Thermostat::calculateScalingFactor(double newTemp){
    scalingFactor = std::sqrt(newTemp / temperature);
}

void Thermostat::setTempViaVelocityScaling(){
    for(auto &p : particles){
        std::array<double, 3> newV = ArrayUtils::elementWiseScalarOp(scalingFactor, p.getV(), std::multiplies<>());
        p.setV(newV);
    }
}

double Thermostat::getKineticEnergy() { return kineticEnergy; }
double Thermostat::getOldTemp() { return oldTemperature; }
double Thermostat::getTemp() { return temperature; }
double Thermostat::getScalingFactor() { return scalingFactor; }
ParticleContainer &Thermostat::getParticles() {return particles; }