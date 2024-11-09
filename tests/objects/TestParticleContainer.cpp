#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>

// Test initializing an empty particle container.
TEST(ParticleContainerTests, InitializeEmpty) {
    ParticleContainer pc;
    EXPECT_EQ(pc.size(), 0);
    EXPECT_TRUE(pc.isEmpty());
}

// Test initializing a particle container with a given capacity.
TEST(ParticleContainerTests, InitializeReserved) {
    ParticleContainer pc(5);
    EXPECT_EQ(pc.size(), 0);
    EXPECT_EQ(pc.getParticles().capacity(), 5);
    EXPECT_TRUE(pc.isEmpty());
}

// Test adding pre-existing particles to a particle container.
TEST(ParticleContainerTests, AddParticlesExisting) {
    ParticleContainer pc(3);
    Particle p0(1);
    Particle p1(2);
    Particle p2(3);

    pc.addParticle(p0);
    ASSERT_EQ(pc.size(), 1);
    pc.addParticle(p1);
    ASSERT_EQ(pc.size(), 2);
    pc.addParticle(p2);
    ASSERT_EQ(pc.size(), 3);

    EXPECT_TRUE(pc[0] == p0);
    EXPECT_TRUE(pc[1] == p1);
    EXPECT_TRUE(pc[2] == p2);

    EXPECT_FALSE(pc.isEmpty());
}

// Test adding particles by their attributes to a particle container.
TEST(ParticleContainerTests, AddParticlesNew) {
    ParticleContainer pc(2);

    pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
    ASSERT_EQ(pc.size(), 1);
    pc.addParticle({0, 0, 0}, {0, 0, 0}, 2);
    ASSERT_EQ(pc.size(), 2);

    EXPECT_EQ(pc[0].getM(), 1);
    EXPECT_EQ(pc[1].getM(), 2);

    EXPECT_FALSE(pc.isEmpty());
}

// Test accessing a specific object (in bounds).
TEST(ParticleContainerTests, GetParticleInBounds) {
    ParticleContainer pc(1);
    Particle p(42);
    pc.addParticle(p);

    EXPECT_TRUE(pc.get(0) == p);
}

// Test accessing a specific object (out of bounds).
TEST(ParticleContainerTests, GetParticleOutOfBounds) {
    ParticleContainer pc;
    EXPECT_DEATH(pc.get(3), "");
}

// Test iterating through an empty particle container.
TEST(ParticleContainerTests, IterateEmpty) {
    ParticleContainer pc;
    for (auto &p : pc) {
        FAIL() << "Should not reach this!";
    }
}

// Test iterating through a non-empty particle container.
TEST(ParticleContainerTests, IterateNonEmpty) {
    ParticleContainer pc(3);
    Particle p0(0);
    Particle p1(1);
    Particle p2(2);
    pc.addParticle(p0);
    pc.addParticle(p1);
    pc.addParticle(p2);
    int index = 0;

    for (auto &p : pc) {
        EXPECT_EQ(p.getType(), index);
        ++index;
    }

    EXPECT_EQ(pc.size(), index);
}

// Test iterating through pairs of an empty particle container.
TEST(ParticleContainerTests, IteratePairEmpty) {
    ParticleContainer pc;
    for (auto pair = pc.beginPairs(); pair != pc.endPairs(); ++pair) {
        FAIL() << "Should not reach this!";
    }
}

// Test iterating through pairs of a non-empty particle container.
TEST(ParticleContainerTests, IteratePairNonEmpty) {
    ParticleContainer pc(3);
    Particle p0(0);
    Particle p1(1);
    Particle p2(2);
    pc.addParticle(p0);
    pc.addParticle(p1);
    pc.addParticle(p2);
    const std::array<int, 6> outerTypes{0, 0, 1, 1, 2, 2};
    const std::array<int, 6> innerTypes{1, 2, 0, 2, 0, 1};
    int index = 0;

    for (auto pair = pc.beginPairs(); pair != pc.endPairs(); ++pair) {
        auto &l = (*pair).first;
        auto &r = (*pair).second;

        EXPECT_EQ(l.getType(), outerTypes[index]);
        EXPECT_EQ(r.getType(), innerTypes[index]);

        ++index;
    }
}