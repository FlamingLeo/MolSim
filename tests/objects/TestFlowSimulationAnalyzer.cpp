#include "objects/FlowSimulationAnalyzer.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <gtest/gtest.h>
#include <math.h>
#include <vector>

class FlowSimulationAnalyzerTests : public ::testing::Test {
  protected:
    ParticleContainer pc;

    void SetUp() override {
        pc.addParticle({0, 0, 0}, {2, 0, 2}, 1);
        pc.addParticle({1, 1, 1}, {2, 2, 0}, 1);
        pc.addParticle({1, 1, 1}, {3, 4, 0}, 1);
        pc.addParticle({2, 2, 2}, {2, 0, 2}, 1);
        pc.addParticle({5, 5, 5}, {4, 4, 4}, 1);
    }
};

// Test calculating the size of a bin and lenghts of density and velocities vectors.
TEST_F(FlowSimulationAnalyzerTests, CalculateAnalyzerComponents) {
    constexpr int binNumber = 3;
    constexpr double leftWallPosX = 0;
    constexpr double rightWallPosX = 6;
    constexpr int n_analzer = 10;
    FlowSimulationAnalyzer a(pc, binNumber, leftWallPosX, rightWallPosX, n_analzer);

    EXPECT_EQ(a.getBinSize(), 2);
    EXPECT_EQ(a.getDensities().size(), 3);
    EXPECT_EQ(a.getVelocities().size(), 3);
}

// Test calculating densities and velocities of a particle system.
TEST_F(FlowSimulationAnalyzerTests, CalculateDensitiesAndVelocities) {
    constexpr int binNumber = 3;
    constexpr double leftWallPosX = 0;
    constexpr double rightWallPosX = 6;
    constexpr int n_analzer = 10;
    FlowSimulationAnalyzer a(pc, binNumber, leftWallPosX, rightWallPosX, n_analzer);

    a.calculateDensitiesAndVelocities();

    EXPECT_EQ(a.getDensities()[0], 3);
    EXPECT_EQ(a.getDensities()[1], 1);
    EXPECT_EQ(a.getDensities()[2], 1);

    EXPECT_EQ(a.getVelocities()[0], 2);
    EXPECT_EQ(a.getVelocities()[1], 0);
    EXPECT_EQ(a.getVelocities()[2], 4);
}
