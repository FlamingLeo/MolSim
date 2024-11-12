#include "simulations/Verlet.h"
#include <gtest/gtest.h>

class VerletTests : public ::testing::Test {
  protected:
    Arguments args;
    ParticleContainer pc;

    void SetUp() override {
        args.startTime = 0;
        args.endTime = 5;
        args.delta_t = 0.5;
        args.type = WriterType::NIL;
        args.itFreq = 0x7FFFFFFF;

        pc.reserve(4);
        pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
        pc.addParticle({0, 1, 0}, {-1, 0, 0}, 3e-06);
        pc.addParticle({0, 5.36, 0}, {-0.425, 0, 0}, 9.55e-4);
        pc.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1.0e-14);
    }
};

// Test the Verlet simulation against both force calculation implementations, aswell as against precomputed values.
TEST_F(VerletTests, RunSimulation) {
    Verlet v0{pc, args, 0};
    Verlet v1{pc, args, 1};

    v0.runSimulation();
    v1.runSimulation();

    constexpr std::array<std::array<double, 3>, 4> x = {
        {{-6.90564e-05, 0.000372863, 0}, {0.211459, -1.00777, 0}, {-2.06906, 4.97587, 0}, {34.7407, 0.147987, 0}}};
    constexpr std::array<std::array<double, 3>, 4> v = {{{-3.78132e-05, 0.000151248, 0},
                                                         {0.897626, 0.449357, 0},
                                                         {-0.391366, -0.159787, 0},
                                                         {-0.00393764, 0.0295917, 0}}};
    constexpr std::array<std::array<double, 3>, 4> f = {{{-1.20479e-05, 2.76018e-05, 0},
                                                         {-5.80593e-07, 2.76706e-06, 0},
                                                         {1.26284e-05, -3.03688e-05, 0},
                                                         {-8.29223e-18, -3.43042e-20, 0}}};
    constexpr std::array<std::array<double, 3>, 4> oldF = {{{-1.19523e-05, 2.86396e-05, 0},
                                                            {5.18972e-07, 2.2179e-06, 0},
                                                            {1.14333e-05, -3.08575e-05, 0},
                                                            {-8.29145e-18, -3.07573e-20, 0}}};
    constexpr std::array<double, 4> m = {1, 3e-06, 0.000955, 1e-14};
    constexpr std::array<int, 4> type = {0, 0, 0, 0};
    constexpr double eps = 0.0001;

    ParticleContainer &p0 = v0.getParticles();
    ParticleContainer &p1 = v1.getParticles();
    ASSERT_EQ(p0, p1);

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_NEAR(p0[i].getX()[j], x[i][j], eps);
            EXPECT_NEAR(p0[i].getV()[j], v[i][j], eps);
            EXPECT_NEAR(p0[i].getF()[j], f[i][j], eps);
            EXPECT_NEAR(p0[i].getOldF()[j], oldF[i][j], eps);
        }
        EXPECT_NEAR(p0[i].getM(), m[i], eps);
        EXPECT_NEAR(p0[i].getType(), type[i], eps);
    }
}