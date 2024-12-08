#pragma once
#include "ParticleContainer.h"

class Thermostat {
  private:
    int dimension;

    double kineticEnergy;

    double oldTemperature;

    double temperature;

    double scalingFactor;

    ParticleContainer &particles;

  public:
    Thermostat(ParticleContainer &particles, int dimension);

    void calculateKineticEnergy();

    void calculateTemp();

    void calculateScalingFactor(double newTemp);

    void setTempViaVelocityScaling();

    void setTempViaGradualScaling();

    double getKineticEnergy();

    double getOldTemp();

    double getTemp();

    double getScalingFactor();

    ParticleContainer &getParticles();
};