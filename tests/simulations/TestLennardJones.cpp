#include "simulations/LennardJones.h"
#include <gtest/gtest.h>

class LennardJonesTests : public ::testing::Test {
  protected:
    Arguments args;
    ParticleContainer pc;

    void SetUp() override {
        args.startTime = 0;
        args.endTime = 5;
        args.delta_t = 0.5;
        args.type = WriterType::NIL;
        args.itFreq = 0x7FFFFFFF;

        // initialize LJ simulation by passing the particle data directly
        // this is done to prevent the randomness from causing tests to differ
        pc.reserve(8);
        constexpr std::array<std::array<double, 3>, 8> x = {{{0, 0, 0},
                                                             {1.1225, 0, 0},
                                                             {0, 1.1225, 0},
                                                             {1.1225, 1.1225, 0},
                                                             {0, 0, 0},
                                                             {1.1225, 0, 0},
                                                             {0, 1.1225, 0},
                                                             {1.1225, 1.1225, 0}}};
        constexpr std::array<std::array<double, 3>, 8> v = {{{-0.00373932, 0.0738478, 0},
                                                             {-0.241207, 0.151752, 0},
                                                             {-0.350797, -0.00591154, 0},
                                                             {-0.499377, 0.0744257, 0},
                                                             {0.00658191, -10.141, 0},
                                                             {-0.0256702, -10.1843, 0},
                                                             {0.0270829, -10.2618, 0},
                                                             {-0.199718, -10.232, 0}}};
        constexpr std::array<double, 8> m = {1, 1, 1, 1, 1, 1, 1, 1};
        for (size_t i = 0; i < 8; ++i)
            pc.addParticle(x[i], v[i], m[i]);
    }
};

// Test the LJ simulation against both force calculation implementations, aswell as against precomputed values.
TEST_F(LennardJonesTests, RunSimulation) {
    LennardJones lj0{pc, args, 0};
    LennardJones lj1{pc, args, 1};

    lj0.runSimulation();
    lj1.runSimulation();

    constexpr std::array<std::array<double, 3>, 8> x = {{{-231.609, -17.5595, 0},
                                                         {243.172, -9.70074, 0},
                                                         {-94.5435, 12.0116, 0},
                                                         {81.5643, 18.0714, 0},
                                                         {-2.93594, -109.921, 0},
                                                         {6.20039, -59.167, 0},
                                                         {-219.191, 9.06269, 0},
                                                         {215.399, -40.9324, 0}}};
    constexpr std::array<std::array<double, 3>, 8> v = {{{-51.4175, -3.9327, 0},
                                                         {53.8155, -2.17259, 0},
                                                         {-20.9707, 2.42045, 0},
                                                         {17.9315, 3.75816, 0},
                                                         {-0.653154, -23.3001, 0},
                                                         {1.13126, -12.0167, 0},
                                                         {-48.7629, 2.92707, 0},
                                                         {47.6392, -8.20863, 0}}};
    constexpr std::array<std::array<double, 3>, 8> f = {{{2.68734e-09, 5.76135e-09, 0},
                                                         {-3.58047e-10, -4.02589e-10, 0},
                                                         {-7.97083e-14, -2.3845e-13, 0},
                                                         {-4.34717e-13, -5.95135e-13, 0},
                                                         {2.19141e-11, 1.21861e-10, 0},
                                                         {-2.16479e-11, -1.21091e-10, 0},
                                                         {-2.68697e-09, -5.76132e-09, 0},
                                                         {3.57919e-10, 4.02626e-10, 0}}};
    constexpr std::array<std::array<double, 3>, 8> oldF = {{{6.97688e-09, 1.45902e-08, 0},
                                                            {-7.59526e-10, -8.67996e-10, 0},
                                                            {-2.03619e-13, -5.34474e-13, 0},
                                                            {-9.46967e-13, -1.31972e-12, 0},
                                                            {5.0563e-11, 2.76958e-10, 0},
                                                            {-4.99819e-11, -2.75254e-10, 0},
                                                            {-6.97602e-09, -1.45901e-08, 0},
                                                            {7.59237e-10, 8.68081e-10, 0}}};
    constexpr std::array<double, 8> m = {1, 1, 1, 1, 1, 1, 1, 1};
    constexpr std::array<int, 8> type = {0, 0, 0, 0, 0, 0, 0, 0};
    constexpr double eps = 0.001;

    ParticleContainer &p0 = lj0.getParticles();
    ParticleContainer &p1 = lj1.getParticles();
    ASSERT_EQ(p0, p1);

    for (size_t i = 0; i < 8; ++i) {
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