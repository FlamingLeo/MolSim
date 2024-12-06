#include "objects/Disc.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>
#include <math.h>

// Test initializing the disc particles (i.e. if the positions are correct, if the force effective is 0, if the
// masses of all the particles are the same and if the positions are correct).
TEST(DiscTests, InitializeDiscRadiusOne) {
    constexpr size_t num = 5;
    constexpr std::array<std::array<double, 3>, num> positions = {
        {{0, -1, 0}, {-1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}}};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    ParticleContainer pc;
    Disc d{pc, {0, 0, 0}, 1, {1., 2., 3.}, 1., 1.};
    d.initializeDisc();

    ASSERT_EQ(d.getParticles().size(), num);
    for (size_t i = 0; i < num; ++i) {
        Particle &p = d.getParticles()[i];
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
        EXPECT_EQ(p.getX(), positions[i]);
    }
}

// Test initializing a disc with radius zero.
TEST(DiscTests, InitializeDiscRadiusZero) {
    constexpr std::array<double, 3> position = {0, 0, 0};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    ParticleContainer pc;
    Disc d{pc, {0, 0, 0}, 0, {1., 2., 3.}, 1., 1.};
    d.initializeDisc();

    ASSERT_EQ(d.getParticles().size(), 1);
    Particle &p = d.getParticles()[0];
    EXPECT_EQ(p.getF(), f);
    EXPECT_EQ(p.getOldF(), oldF);
    EXPECT_EQ(p.getM(), 1.0);
    EXPECT_EQ(p.getX(), position);
}

// Test attempting to create discs with invalid values.
TEST(DiscTests, CreateDiscInvalid) {
    ParticleContainer pc;
    EXPECT_DEATH({ Disc d(pc, {0, 0, 0}, -1, {1., 2., 3.}, 1., 1.); }, ""); // radius negative
    EXPECT_DEATH({ Disc d(pc, {0, 0, 0}, 1, {1., 2., 3.}, 0., 1.); }, "");  // spacing zero
    EXPECT_DEATH({ Disc d(pc, {0, 0, 0}, 1, {1., 2., 3.}, -1., 1.); }, ""); // spacing negative
}

// Test the equality operator on two Disc objects.
TEST(DiscTests, Equality) {
    ParticleContainer pc;
    Disc d1{pc, {0, 0, 0}, 1, {1., 2., 3.}, 1., 1.};
    Disc d2{pc, {0, 0, 0}, 1, {1., 2., 3.}, 1., 1.};
    Disc d3{pc, {1, 2, 3}, 1, {1., 2., 3.}, 1., 1.};
    EXPECT_TRUE(d1 == d2);
    EXPECT_FALSE(d1 == d3);
    EXPECT_FALSE(d2 == d3);
}

// Test the inequality operator on two Disc objects.
TEST(DiscTests, Inequality) {
    ParticleContainer pc;
    Disc d1{pc, {0, 0, 0}, 1, {1., 2., 3.}, 1., 1.};
    Disc d2{pc, {0, 0, 0}, 1, {1., 2., 3.}, 1., 1.};
    Disc d3{pc, {1, 2, 3}, 1, {1., 2., 3.}, 1., 1.};
    EXPECT_FALSE(d1 != d2);
    EXPECT_TRUE(d1 != d3);
    EXPECT_TRUE(d2 != d3);
}
