#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>

// Test initializing an empty cuboid.
TEST(CuboidTests, InitializeCuboidParticlesEmpty) {
    ParticleContainer pc;
    Cuboid c{pc, {0., 0., 0.}, {0, 0, 0}, {0., 0., 0.}, 0., 0.};
    c.initializeParticles();

    ASSERT_EQ(c.getParticles().size(), 0);
}

// Test initializing the cuboid particles (i.e. if the positions are correct, if the force effective is 0 and if the
// masses of all the particles are the same).
TEST(CuboidTests, InitializeCuboidParticlesNonEmpty) {
    ParticleContainer pc;
    Cuboid c{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    c.initializeParticles();

    constexpr std::array<std::array<double, 6>, 3> expectedX = {
        {{1., 1., 1., 1., 1., 1.}, {2., 3., 2., 3., 2., 3.}, {3., 3., 4., 4., 5., 5.}}};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double eps = 0.00001;

    ASSERT_EQ(c.getParticles().size(), 6);

    for (size_t i = 0; i < 6; ++i) {
        Particle &p = c.getParticles()[i];

        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(p.getX()[j], expectedX[j][i]);
        }
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
    }
}

// Test the equality operator on two Cuboid objects.
TEST(CuboidTests, Equality) {
    ParticleContainer pc;
    Cuboid c1{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    Cuboid c2{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    Cuboid c3{pc, {4., 5., 6.}, {7, 8, 9}, {10., 11., 12.}, 2., 2.};
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
    EXPECT_FALSE(c2 == c3);
}

// Test the inequality operator on two Cuboid objects.
TEST(CuboidTests, Inequality) {
    ParticleContainer pc;
    Cuboid c1{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    Cuboid c2{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    Cuboid c3{pc, {4., 5., 6.}, {7, 8, 9}, {10., 11., 12.}, 2., 2.};
    EXPECT_FALSE(c1 != c2);
    EXPECT_TRUE(c1 != c3);
    EXPECT_TRUE(c2 != c3);
}
