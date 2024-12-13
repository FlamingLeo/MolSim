#include "Thermostat.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <cmath>

Thermostat::Thermostat(ParticleContainer &particles, int dimension, double T_init, double T_target, int n_thermostat, double delta_T) 
: particles{particles}, dimension{dimension}, T_init{T_init}, T_target{T_target}, n_thermostat{n_thermostat}, delta_T{delta_T} {
    scaling_limit = true;
}

Thermostat::Thermostat(ParticleContainer &particles, int dimension, double T_init, double T_target, int n_thermostat) 
: particles{particles}, dimension{dimension}, T_init{T_init}, T_target{T_target}, n_thermostat{n_thermostat} {
    scaling_limit = false;
}

Thermostat::Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat, double delta_T) 
: particles{particles}, dimension{dimension}, T_init{T_init}, n_thermostat{n_thermostat}, delta_T{delta_T} {
    scaling_limit = true;
    T_target = T_init;
}

Thermostat::Thermostat(ParticleContainer &particles, int dimension, double T_init, double T_target, int n_thermostat) 
: particles{particles}, dimension{dimension}, T_init{T_init}, n_thermostat{n_thermostat} {
    scaling_limit = false;
    T_target = T_init;
}

void Thermostat::calculateKineticEnergy() {
    double sum = 0;
    for (auto &p : particles) {
        sum += p.getM() * ArrayUtils::L2Norm(p.getV());
    }
    kineticEnergy = sum / 2;
}

void Thermostat::calculateTemp() { temperature = 2 * kineticEnergy / (dimension * particles.activeSize()); }

void Thermostat::calculateScalingFactor(double newTemp) { scalingFactor = std::sqrt(newTemp / temperature); }

void Thermostat::updateSystemTemp(int currentStep) {
    if(currentStep % n_thermostat == 0){
        calculateKineticEnergy();
        calculateTemp();
        if(scaling_limit){
            if(temperature != T_target){
                if(abs(temperature - T_target) < abs(delta_T)){
                    calculateScalingFactor(T_target);
                }
                else{
                    calculateScalingFactor(temperature + delta_T);
                }
            }
        }
        else{
            calculateScalingFactor(T_target);
        }
        for (auto &p : particles) {
            std::array<double, 3> newV = ArrayUtils::elementWiseScalarOp(scalingFactor, p.getV(), std::multiplies<>());
            p.setV(newV);
        }
    }
}

double Thermostat::getKineticEnergy() { return kineticEnergy; }
double Thermostat::getOldTemp() { return oldTemperature; }
double Thermostat::getTemp() { return temperature; }
double Thermostat::getScalingFactor() { return scalingFactor; }
ParticleContainer &Thermostat::getParticles() { return particles; }