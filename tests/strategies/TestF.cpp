#include "strategies/F.h"
#include <gtest/gtest.h>

class ForceTests : public ::testing::Test {
  protected:
    ParticleContainer pc1;
    ParticleContainer pc2;

    void SetUp() override {
        pc1.reserve(3);
        pc2.reserve(3);

        const std::array<Particle, 3> particles = {{{{1., 2., 3.}, {4., 5., 6.}, 7.},
                                                    {{8., 9., 10.}, {11., 12., 13.}, 14.},
                                                    {{15., 16., 17.}, {18., 19., 20.}, 21.}}};

        for (const auto &particle : particles) {
            pc1.addParticle(particle);
            pc2.addParticle(particle);
        }
    }
};

// Test calculating the forces between particles when simulating Verlet integration.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceVerlet) {
    calculateF_Verlet(pc1, 0, 0);
    calculateF_VerletThirdLaw(pc2, 0, 0);

    constexpr std::array<std::array<double, 3>, 3> expectedForces = {
        {{0.529238, 0.529238, 0.529238}, {0.7698, 0.7698, 0.7698}, {-1.29904, -1.29904, -1.29904}}};
    constexpr double eps = 0.00001;

    EXPECT_EQ(pc1, pc2);

    for (size_t i = 0; i < expectedForces.size(); ++i) {
        for (size_t j = 0; j < expectedForces[i].size(); ++j) {
            EXPECT_NEAR(pc1[i].getF()[j], expectedForces[i][j], eps);
        }
    }
}

// Test calculating the forces between particles when simulating the LJ potential.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceLJ) {
    calculateF_LennardJones(pc1, 5, 1);
    calculateF_LennardJonesThirdLaw(pc2, 5, 1);

    constexpr std::array<std::array<double, 3>, 3> expectedForces = {
        {{1.81296e-06, 1.81296e-06, 1.81296e-06}, {0., 0., 0.}, {-1.81296e-06, -1.81296e-06, -1.81296e-06}}};
    constexpr double eps = 0.00001;

    EXPECT_EQ(pc1, pc2);

    for (size_t i = 0; i < expectedForces.size(); ++i) {
        for (size_t j = 0; j < expectedForces[i].size(); ++j) {
            EXPECT_NEAR(pc1[i].getF()[j], expectedForces[i][j], eps);
        }
    }
}