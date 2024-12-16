#pragma once
#include "ParticleContainer.h"
#include <cmath>

class Thermostat {
  private:
    int dimension;

    double kineticEnergy;

    double temperature;

    double scalingFactor;

    double T_init;

    double T_target;

    int n_thermostat;

    double delta_T;

    bool limitScaling;

    bool initBrownianMotion;

    ParticleContainer &particles;

  public:
    Thermostat(ParticleContainer &particles);

    Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat, double T_target,
               double delta_T, bool initBrownianMotion);

    ~Thermostat();

    void initialize(int dimension, double T_init, int n_thermostat, double T_target, double delta_T,
                    bool initBrownianMotion);

    void initializeBrownianMotion();

    void calculateKineticEnergy();

    void calculateTemp();

    void calculateScalingFactor(double newTemp);

    void updateSystemTemp(int currentStep);

    double getKineticEnergy() const;

    double getOldTemp() const;

    double getTemp() const;

    double getInitTemp() const;

    double getTargetTemp() const;

    double getDeltaT() const;

    double getScalingFactor() const;

    int getTimestep() const;

    ParticleContainer &getParticles() const;
};