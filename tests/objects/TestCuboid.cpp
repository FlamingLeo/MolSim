#include "objects/Cuboid.h"
#include <gtest/gtest.h>

// Test updating the position of the cuboid.
TEST(CuboidTests, SetX) {
    constexpr std::array x{1.0, 2.0, 3.0};
    Cuboid c;
    c.setPosition(x);
    EXPECT_EQ(c.getPosition(), x);
}

// Test updating the size of the cuboid.
TEST(CuboidTests, SetN) {
    constexpr std::array N{1, 2, 3};
    Cuboid c;
    c.setSize(N);
    EXPECT_EQ(c.getSize(), N);
}

// Test updating the distance of the cuboid particles.
TEST(CuboidTests, SetH) {
    constexpr double h = 1.0;
    Cuboid c;
    c.setH(h);
    EXPECT_EQ(c.getH(), h);
}

// Test updating the mass of one particle.
TEST(CuboidTests, SetM) {
    constexpr double m = 1.0;
    Cuboid c;
    c.setM(m);
    EXPECT_EQ(c.getM(), m);
}

// Test updating the velocity of the cuboid particles.
TEST(CuboidTests, SetV) {
    constexpr std::array v{1.0, 2.0, 3.0};
    Cuboid c;
    c.setV(v);
    EXPECT_EQ(c.getV(), v);
}

// Test updating the mean velocity.
TEST(CuboidTests, SetMeanVelocity) {
    constexpr double mv = 1.0;
    Cuboid c;
    c.setMeanVelocity(mv);
    EXPECT_EQ(c.getMeanVelocity(), mv);
}
