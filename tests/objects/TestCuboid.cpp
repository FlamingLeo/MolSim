#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>
/*
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

// Test initializing the cuboid particles.
TEST(CuboidTests, InitializeParticles) {
    ParticleContainer pc;
    Cuboid c{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    c.initializeParticles();

    // TODO: check velocity, independent of executable
    constexpr std::array x0 = {1., 1., 1., 1., 1., 1.};
    constexpr std::array x1 = {2., 3., 2., 3., 2., 3.};
    constexpr std::array x2 = {3., 3., 4., 4., 5., 5.};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double eps = 0.00001;

    ASSERT_EQ(c.getParticles().size(), 6);

    for (size_t i = 0; i < 6; ++i) {
        Particle &p = c.getParticles()[i];
        EXPECT_EQ(p.getX()[0], x0[i]);
        EXPECT_EQ(p.getX()[1], x1[i]);
        EXPECT_EQ(p.getX()[2], x2[i]);
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
    }
}
 */