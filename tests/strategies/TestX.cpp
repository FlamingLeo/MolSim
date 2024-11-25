#include "strategies/PositionCalculation.h"
#include <gtest/gtest.h>

// Test calculating the new position of a particle.
TEST(PositionTests, UpdatePositions) {
    constexpr std::array xBefore{1., 2., 3.};
    constexpr std::array xAfter{3., 4.5, 6.};

    ParticleContainer pc(1);
    pc.addParticle(xBefore, {4., 5., 6.}, 7.);
    calculateX(pc, 0.5);

    EXPECT_EQ(pc[0].getX(), xAfter);
}