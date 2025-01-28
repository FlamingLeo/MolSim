#include "strategies/ForceCalculation.h"
#include <gtest/gtest.h>

class ForceTests : public ::testing::Test {
  protected:
    ParticleContainer pc1;
    ParticleContainer pc2;

    void SetUp() override {
        pc1.reserve(3);
        pc2.reserve(3);

        const std::array<Particle, 3> particles = {{{{1., 2., 3.}, {4., 5., 6.}, 7., 0, 5., 1.},
                                                    {{8., 9., 10.}, {11., 12., 13.}, 14., 0, 5., 1.},
                                                    {{15., 16., 17.}, {18., 19., 20.}, 21., 0, 5., 1.}}};

        for (const auto &particle : particles) {
            pc1.addParticle(particle);
            pc2.addParticle(particle);
        }
    }
};

// Test calculating the forces between particles when simulating gravitational orbit.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceGravity) {
    calculateF_Gravity(pc1, 1);

    constexpr std::array<std::array<double, 3>, 3> expectedF = {
        {{0.529238, 0.529238, 0.529238}, {0.7698, 0.7698, 0.7698}, {-1.29904, -1.29904, -1.29904}}};
    constexpr double eps = 0.00001;

    for (size_t i = 0; i < expectedF.size(); ++i) {
        for (size_t j = 0; j < expectedF[i].size(); ++j) {
            EXPECT_NEAR(pc1[i].getF()[j], expectedF[i][j], eps);
        }
    }
}

// Test calculating the forces between particles when simulating the LJ potential.
// More specifically, test if both the naive approach and the approach using Newton's third law are identical.
TEST_F(ForceTests, UpdateForceLJ) {
    calculateF_LennardJones(pc1, 1);

    constexpr std::array<std::array<double, 3>, 3> expectedF = {
        {{1.81296e-06, 1.81296e-06, 1.81296e-06}, {0., 0., 0.}, {-1.81296e-06, -1.81296e-06, -1.81296e-06}}};
    constexpr double eps = 0.00001;

    for (size_t i = 0; i < expectedF.size(); ++i) {
        for (size_t j = 0; j < expectedF[i].size(); ++j) {
            EXPECT_NEAR(pc1[i].getF()[j], expectedF[i][j], eps);
        }
    }
}

// Test the results of both parallelization strategies.
// Assuming correct parallelization, the results should be identical.
TEST_F(ForceTests, Parallelization) {
    CellContainer cc1{{21, 21, 1},
                      {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                       BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE},
                      1,
                      pc1,
                      2};
    CellContainer cc2{{21, 21, 1},
                      {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                       BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE},
                      1,
                      pc2,
                      2};
    calculateF_LennardJones_LC(pc1, 0, &cc1);
    calculateF_LennardJones_LC_task(pc2, 0, &cc2);
    ASSERT_EQ(pc1.size(), pc2.size());
    for (size_t i = 0; i < pc1.size(); ++i) {
        EXPECT_EQ(pc1[i], pc2[i]);
    }
}

//Tests calculateF_Membrane_LC (i.e. forces for membranes )
TEST_F(ForceTests, ForceMembrane) {
    constexpr double eps = 0.00001;
    ParticleContainer pc3(2);
    pc3.setSpecialForceLimit(100);

    constexpr std::array<std::array<double, 3>, 8> force = {{{1., 0., 1.},
                          {-1., 0., 2.}, {1., 1., 1.}, {-1., -1., 2.}, {-24., 0., 1.}, {24., 0., 2.}, {0., 0., 1.}, {0., 0., 2.}}};


    Cuboid cub{pc3, {5., 5., 5.}, {2, 1, 1}, {0., 0., 0.}, 2.0, 1., 5, 1, 1,
             1, 1.0, 1.0};

    cub.initialize(3);
    cub.initializeNeighbours();

    pc3[0].setFZUP(1);
    pc3[1].setFZUP(2);

    CellContainer cc{{10, 10, 10},
                      {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                       BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE},
                      2.5,
                      pc3,
                      3};

    //direct neighbour
    EXPECT_EQ(pc3[0].getDirectNeighbours().size(), 1);
    EXPECT_EQ(pc3[1].getDirectNeighbours().size(), 1);

    calculateF_Membrane_LC(pc3, 0, &cc);

    EXPECT_EQ(pc3[0].getF(), force[0]);
    EXPECT_EQ(pc3[1].getF(), force[1]);

    //diagonal neighbour
    pc3[0].setF({0., 0., 0.});
    pc3[1].setF({0., 0., 0.});

    pc3[1].setX({7., 7., 5.});

    pc3[0].setDirectNeighbours({});
    pc3[1].setDirectNeighbours({});
    pc3[0].setDiagonalNeighbours({{pc3[1]}});
    pc3[1].setDiagonalNeighbours({{pc3[0]}});

    calculateF_Membrane_LC(pc3, 0, &cc);

    EXPECT_EQ(pc3[0].getF(), force[2]);
    EXPECT_EQ(pc3[1].getF(), force[3]);

    //too close particles
    pc3[0].setF({0., 0., 0.});
    pc3[1].setF({0., 0., 0.});

    pc3[1].setX({6, 5., 5.});

    pc3[0].setDirectNeighbours({});
    pc3[1].setDirectNeighbours({});
    pc3[0].setDiagonalNeighbours({});
    pc3[1].setDiagonalNeighbours({});

    calculateF_Membrane_LC(pc3, 0, &cc);

    EXPECT_EQ(pc3[0].getF(), force[4]);
    EXPECT_EQ(pc3[1].getF(), force[5]);

    //not neighbours,far away
    pc3[0].setF({0., 0., 0.});
    pc3[1].setF({0., 0., 0.});

    pc3[1].setX({8, 5., 5.});

    calculateF_Membrane_LC(pc3, 0, &cc);

    EXPECT_EQ(pc3[0].getF(), force[6]);
    EXPECT_EQ(pc3[1].getF(), force[7]);
}
