#include "strategies/V.h"
#include <gtest/gtest.h>

// Test calculating the new velocity of a particle.
TEST(VelocityTests, UpdateVelocity) {
    constexpr std::array vBefore{4., 5., 6.};
    constexpr std::array vAfter{4.17857, 5.25, 6.32143};
    constexpr std::array oldF{1., 2., 3.};
    constexpr std::array f{4., 5., 6.};
    constexpr double eps = 0.00001;

    ParticleContainer pc(1);
    pc.addParticle({1., 2., 3.}, vBefore, 7.);
    pc[0].setOldF(oldF);
    pc[0].setF(f);
    calculateV(pc, 0.5);

    for (size_t i = 0; i < vAfter.size(); ++i) {
        EXPECT_NEAR(pc[0].getV()[i], vAfter[i], eps);
    }
}