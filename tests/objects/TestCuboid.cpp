#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>

// Test initializing an empty cuboid.
TEST(CuboidTests, InitializeCuboidParticlesEmpty) {
    ParticleContainer pc;
    Cuboid c{pc, {0., 0., 0.}, {0, 0, 0}, {0., 0., 0.}, 0., 0.};
    c.initialize();

    ASSERT_EQ(c.getParticles().size(), 0);
}

// Test initializing the cuboid particles (i.e. if the positions are correct, if the force effective is 0 and if the
// masses of all the particles are the same).
TEST(CuboidTests, InitializeCuboidParticlesNonEmpty) {
    ParticleContainer pc;
    Cuboid c{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    c.initialize();

    constexpr std::array<std::array<double, 6>, 3> expectedX = {
        {{1., 1., 1., 1., 1., 1.}, {2., 3., 2., 3., 2., 3.}, {3., 3., 4., 4., 5., 5.}}};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double eps = 0.00001;

    ASSERT_EQ(c.getParticles().size(), 6);

    for (size_t i = 0; i < 6; ++i) {
        auto p = c.getParticles()[i];

        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(p.getX()[j], expectedX[j][i]);
        }
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
    }
}

TEST(CuboidTests, InitializeCuboidMembrane) {
    ParticleContainer pc;
    Cuboid c{pc, {1., 1., 1.}, {3, 3, 1}, {1., 2., 3.}, 1.0, 1., 5, 1, 1, 300, 1.0, 0.8};
    c.initialize();
    c.initializeNeighbours();

    constexpr std::array<std::array<double, 9>, 3> expectedX = {{{1., 2., 3., 1., 2., 3., 1., 2., 3.},
                                                                 {1., 1., 1., 2., 2., 2., 3., 3., 3.},
                                                                 {1., 1., 1., 1., 1., 1., 1., 1., 1.}}};
    constexpr std::array<int, 9> expectedNumDirectNeighbours = {2, 3, 2, 3, 4, 3, 2, 3, 2};
    constexpr std::array<int, 9> expectedNumDiagonalNeighbours = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double eps = 0.00001;

    ASSERT_EQ(c.getParticles().size(), 9);
    // check membrane particle initialization
    for (size_t i = 0; i < 9; ++i) {
        auto p = c.getParticles()[i];

        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(p.getX()[j], expectedX[j][i]);
        }
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
        EXPECT_EQ(p.getFZUP(), 0.);
        EXPECT_EQ(p.getK(), 300);
        EXPECT_EQ(p.getR0(), 1.);
        EXPECT_EQ(p.getType(), 5);
        EXPECT_EQ(c.getParticles()[i].getDirectNeighbours().size(), expectedNumDirectNeighbours[i]);
        EXPECT_EQ(c.getParticles()[i].getDiagonalNeighbours().size(), expectedNumDiagonalNeighbours[i]);
    }
    // check the references of some of the particles
    /* particle 0 */
    EXPECT_EQ(c.getParticles()[0].getDirectNeighbours()[0].get(), c.getParticles()[1]);
    EXPECT_EQ(c.getParticles()[0].getDirectNeighbours()[1].get(), c.getParticles()[3]);
    EXPECT_EQ(c.getParticles()[0].getDiagonalNeighbours()[0].get(), c.getParticles()[4]);
    /* particle 4*/
    EXPECT_EQ(c.getParticles()[4].getDirectNeighbours()[0].get(), c.getParticles()[3]);
    EXPECT_EQ(c.getParticles()[4].getDirectNeighbours()[1].get(), c.getParticles()[5]);
    EXPECT_EQ(c.getParticles()[4].getDirectNeighbours()[2].get(), c.getParticles()[1]);
    EXPECT_EQ(c.getParticles()[4].getDirectNeighbours()[3].get(), c.getParticles()[7]);

    EXPECT_EQ(c.getParticles()[4].getDiagonalNeighbours()[0].get(), c.getParticles()[6]);
    EXPECT_EQ(c.getParticles()[4].getDiagonalNeighbours()[1].get(), c.getParticles()[0]);
    EXPECT_EQ(c.getParticles()[4].getDiagonalNeighbours()[2].get(), c.getParticles()[8]);
    EXPECT_EQ(c.getParticles()[4].getDiagonalNeighbours()[3].get(), c.getParticles()[2]);
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
