#include "objects/Particle.h"
#include <gtest/gtest.h>

// Test creating a valid particle.
TEST(ParticleTests, CreateParticleValid) {
    constexpr std::array x{1.0, 2.0, 3.0};
    constexpr std::array v{4.0, 5.0, 6.0};
    constexpr std::array f{7.0, 8.0, 9.0};
    constexpr std::array zero{0., 0., 0.};
    constexpr double m = 10.0;
    constexpr double type = 11.0;

    // default constructor
    Particle p0;
    EXPECT_EQ(p0.getF(), zero);
    EXPECT_EQ(p0.getOldF(), zero);
    EXPECT_EQ(p0.getM(), 1.0);
    EXPECT_EQ(p0.getType(), 0);
    EXPECT_EQ(p0.getV(), zero);
    EXPECT_EQ(p0.getX(), zero);

    // pass arguments directly
    Particle p1(x, v, m, type);
    EXPECT_EQ(p1.getF(), zero);
    EXPECT_EQ(p1.getOldF(), zero);
    EXPECT_EQ(p1.getM(), m);
    EXPECT_EQ(p1.getType(), type);
    EXPECT_EQ(p1.getV(), v);
    EXPECT_EQ(p1.getX(), x);

    // get attributes from another particle
    Particle p2(p1);
    EXPECT_EQ(p2.getF(), p1.getF());
    EXPECT_EQ(p2.getOldF(), p1.getOldF());
    EXPECT_EQ(p2.getM(), p1.getM());
    EXPECT_EQ(p2.getType(), p1.getType());
    EXPECT_EQ(p2.getV(), p1.getV());
    EXPECT_EQ(p2.getX(), p1.getX());
}

// Test creating invalid particles.
TEST(ParticleTests, CreateParticleInvalid) {
    constexpr std::array x{1.0, 2.0, 3.0};
    constexpr std::array v{4.0, 5.0, 6.0};
    constexpr std::array f{7.0, 8.0, 9.0};
    constexpr double type = 11.0;

    constexpr double m0 = 0.0;
    constexpr double m1 = -2.0;

    EXPECT_DEATH({ Particle p1(x, v, m0, type); }, "");
    EXPECT_DEATH({ Particle p2(x, v, m1, type); }, "");
}

// Test updating the position of the particle.
TEST(ParticleTests, SetX) {
    constexpr std::array x{1.0, 2.0, 3.0};
    Particle p;
    p.setX(x);
    EXPECT_EQ(p.getX(), x);
}

// Test updating the velocity of the particle.
TEST(ParticleTests, SetV) {
    constexpr std::array v{1.0, 2.0, 3.0};
    Particle p;
    p.setV(v);
    EXPECT_EQ(p.getV(), v);
}

// Test updating the force effective on the particle.
TEST(ParticleTests, SetF) {
    constexpr std::array f{1.0, 2.0, 3.0};
    Particle p;
    p.setF(f);
    EXPECT_EQ(p.getF(), f);
}

// Test updating the previous force effective on the particle.
TEST(ParticleTests, SetOldF) {
    constexpr std::array oldF{1.0, 2.0, 3.0};
    Particle p;
    p.setOldF(oldF);
    EXPECT_EQ(p.getOldF(), oldF);
}

// Test zeroing the force effective on this particle.
TEST(ParticleTests, ZeroF) {
    constexpr std::array f{1.0, 2.0, 3.0};
    constexpr std::array zero{0., 0., 0.};
    Particle p;
    p.setF(f);

    p.setFToZero();
    EXPECT_EQ(p.getF(), zero);
}

// Test updating the mass of the particle to a positive number.
TEST(ParticleTests, SetMValid) {
    constexpr double m = 2.0;
    Particle p;
    p.setM(m);
    EXPECT_EQ(p.getM(), m);
}

// Test updating the mass of the particle to an invalid number.
TEST(ParticleTests, SetMInvalid) {
    constexpr double mNegative = -2.0;
    constexpr double mZero = 0.0;
    Particle p;
    EXPECT_DEATH({ p.setM(mNegative); }, "");
    EXPECT_DEATH({ p.setM(mZero); }, "");
}

// Test equality and inequality of two particles when generating a second particle via copy.
TEST(ParticleTests, InEqualityByCopy) {
    constexpr std::array x{1.0, 2.0, 3.0};
    constexpr std::array v{4.0, 5.0, 6.0};
    constexpr std::array f{7.0, 8.0, 9.0};
    constexpr double m = 10.0;
    constexpr double type = 11.0;

    Particle p1(x, v, m, type);
    Particle p2(p1);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 != p2);

    p2.setF(f);
    EXPECT_FALSE(p1 == p2);
    EXPECT_TRUE(p1 != p2);
}

// Test equality and inequality of two particles when generating a second particle using explicit attributes.
TEST(ParticleTests, InEqualityByValues) {
    constexpr std::array x{1.0, 2.0, 3.0};
    constexpr std::array v{4.0, 5.0, 6.0};
    constexpr std::array f{7.0, 8.0, 9.0};
    constexpr double m = 10.0;
    constexpr double type = 11.0;

    Particle p1(x, v, m, type);
    Particle p2(x, v, m, type);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 != p2);

    p2.setF(f);
    EXPECT_FALSE(p1 == p2);
    EXPECT_TRUE(p1 != p2);
}
