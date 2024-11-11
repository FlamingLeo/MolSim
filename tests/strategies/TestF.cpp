#include "strategies/F.h"
#include <gtest/gtest.h>

class ForceTests : public ::testing::Test {
  protected:
    ParticleContainer pc1;
    ParticleContainer pc2;

    void SetUp() override {
        pc1.reserve(3);
        pc2.reserve(3);

        Particle p1{{1., 2., 3.}, {4., 5., 6.}, 7.};
        Particle p2{{8., 9., 10.}, {11., 12., 13.}, 14.};
        Particle p3{{15., 16., 17.}, {18., 19., 20.}, 21.};

        pc1.addParticle(p1);
        pc1.addParticle(p2);
        pc1.addParticle(p3);
        pc2.addParticle(p1);
        pc2.addParticle(p2);
        pc2.addParticle(p3);
    }
};

// Test calculating the forces between particles when simulating Verlet integration.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceVerlet) {
    calculateF_Verlet(pc1, 0, 0);
    calculateF_VerletThirdLaw(pc2, 0, 0);

    EXPECT_EQ(pc1, pc2);
}

// Test calculating the forces between particles when simulating the LJ potential.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceLJ) {
    calculateF_LennardJones(pc1, 5, 1);
    calculateF_LennardJonesThirdLaw(pc2, 5, 1);

    EXPECT_EQ(pc1, pc2);
}