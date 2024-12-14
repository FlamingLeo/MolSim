#pragma once
#include "ParticleContainer.h"

class Thermostat {
  private:
    int dimension;

    double kineticEnergy;

    double oldTemperature;

    double temperature;

    double scalingFactor;

    double T_init;

    double T_target;

    int n_thermostat;

    double delta_T;

    bool scaling_limit;

    ParticleContainer &particles;

  public:
    Thermostat(ParticleContainer &particles, int dimension, double T_init, double T_target, int n_thermostat,
               double delta_T);

    Thermostat(ParticleContainer &particles, int dimension, double T_init, double T_target, int n_thermostat);

    Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat, double delta_T);

    Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat);

    void calculateKineticEnergy();

    void calculateTemp();

    void calculateScalingFactor(double newTemp);

    void updateSystemTemp(int currentStep);

    double getKineticEnergy();

    double getOldTemp();

    double getTemp();

    double getScalingFactor();

    ParticleContainer &getParticles();
};