#include "objects/Thermostat.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>
#include <math.h>

//test calculating kinetic energy, respective temperature and scaling factor
TEST(ThermostatTests, kineticEnergyTemperatureScalingFactor){
    Particle p0{{0, 0, 0}, {2, 0, 2}, 1};
    Particle p1{{1, 1, 1}, {2, 2, 0}, 1};
    ParticleContainer pc;
    pc.addParticle(p0);
    pc.addParticle(p1);
    int dim = 2 ;
    double T_init = 4;
    int n_thermostat = 10;
    double T_target = 20;
    double delta_T = 12.0;
    bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};
    t.calculateKineticEnergy();
    t.calculateTemp();
    t.calculateScalingFactor();
 
    EXPECT_EQ(t.getKineticEnergy(), 8);
    EXPECT_EQ(t.getTemp(), 4);
    EXPECT_EQ(t.getScalingFactor(), 2);
}

//test calculating new velocities after one thermostat application
TEST(ThermostatTests, updateSystem){
    Particle p0{{0, 0, 0}, {2, 0, 2}, 1};
    Particle p1{{1, 1, 1}, {2, 2, 0}, 1};
    ParticleContainer pc;
    pc.addParticle(p0);
    pc.addParticle(p1);
    int dim = 2 ;
    double T_init = 4;
    int n_thermostat = 10;
    double T_target = 20;
    double delta_T = 12.0;
    bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};
    t.updateSystemTemp(100);

    std::array<double, 3> v0 = {4, 0, 4};
    EXPECT_EQ(pc.getParticles()[0].getV(), v0);

    std::array<double, 3> v1 = {4, 4, 0};
    EXPECT_EQ(pc.getParticles()[1].getV(), v1);
}