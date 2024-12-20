#include "objects/ParticleContainer.h"
#include "objects/Thermostat.h"
#include "utils/ArrayUtils.h"
#include <gtest/gtest.h>
#include <math.h>

class ThermostatTests : public ::testing::Test {
  protected:
    ParticleContainer pc;

    void SetUp() override {
        pc.addParticle({0, 0, 0}, {2, 0, 2}, 1);
        pc.addParticle({1, 1, 1}, {2, 2, 0}, 1);
    }
};

// Test calculating kinetic energy, respective temperature and scaling factor.
TEST_F(ThermostatTests, CalculateSystemComponents) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 20;
    constexpr double delta_T = 12.0;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};
    t.calculateKineticEnergy();
    t.calculateTemp();
    t.calculateScalingFactor();

    EXPECT_DOUBLE_EQ(t.getKineticEnergy(), 8);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
    EXPECT_DOUBLE_EQ(t.getScalingFactor(), 2);
}

// Test updating the particle velocities.
TEST_F(ThermostatTests, UpdateVelocity) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 20;
    constexpr double delta_T = 12.0;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};
    t.updateSystemTemp(100);
    ASSERT_DOUBLE_EQ(t.getTemp(), 4);

    constexpr std::array<double, 3> v0 = {4, 0, 4};
    EXPECT_EQ(pc.getParticles()[0].getV(), v0);

    constexpr std::array<double, 3> v1 = {4, 4, 0};
    EXPECT_EQ(pc.getParticles()[1].getV(), v1);
}

// Test heating the particle system gradually with a specified timestep.
TEST_F(ThermostatTests, HeatSystemGradually) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 20;
    constexpr double delta_T = 12.0;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};

    t.updateSystemTemp(10);
    ASSERT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(20);
    ASSERT_DOUBLE_EQ(t.getTemp(), 16);
    t.updateSystemTemp(30);
    ASSERT_DOUBLE_EQ(t.getTemp(), 20);
}

// Test heating the particle system directly without a specified timestep.
TEST_F(ThermostatTests, HeatSystemDirectly) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 20;
    constexpr double delta_T = INFINITY;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};

    t.updateSystemTemp(10);
    ASSERT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(20);
    ASSERT_DOUBLE_EQ(t.getTemp(), 20);
}

// Test cooling the particle system gradually with a specified timestep.
TEST_F(ThermostatTests, CoolSystemGradually) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 0;
    constexpr double delta_T = 2.0;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};

    t.updateSystemTemp(10);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(20);
    EXPECT_DOUBLE_EQ(t.getTemp(), 2);
    t.updateSystemTemp(30);
    EXPECT_LT(t.getTemp(), 1e-9);
}

// Test cooling the particle system directly without a specified timestep.
TEST_F(ThermostatTests, CoolSystemDirectly) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 0;
    constexpr double delta_T = INFINITY;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};

    t.updateSystemTemp(10);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(20);
    EXPECT_DOUBLE_EQ(t.getTemp(), 0);
}

// Test maintaining a system temperature.
TEST_F(ThermostatTests, HoldSystemTemperature) {
    constexpr int dim = 2;
    constexpr double T_init = 4;
    constexpr int n_thermostat = 10;
    constexpr double T_target = 4;
    constexpr double delta_T = INFINITY;
    constexpr bool initBrownianMotion = false;
    Thermostat t{pc, dim, T_init, n_thermostat, T_target, delta_T, initBrownianMotion};

    t.updateSystemTemp(10);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(20);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
    t.updateSystemTemp(30);
    EXPECT_DOUBLE_EQ(t.getTemp(), 4);
}