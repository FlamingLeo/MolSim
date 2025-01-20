#include "strategies/BoundaryConditions.h"
#include "strategies/PositionCalculation.h"
#include <gtest/gtest.h>

// Tests the outflow boundary condition using a single particle in a cell container.
// Upon entering a halo cell, the particle is removed from the cell container and is marked inactive.
TEST(BoundaryConditionTests, Outflow2D) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{10., 10., 1.}, conditions, 1., pc, 2};
        calculateX_LC(pc, delta_t, 0.0, &c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), 0);
        EXPECT_EQ(pc[0].getCellIndex(), -1);
        EXPECT_FALSE(pc[0].isActive());
    };
    test({10.75, 10.75, 0}, {0., 10., 0.}); // north
    test({1.25, 1.25, 0}, {0., -10., 0.});  // south
    test({10.75, 10.75, 0}, {10., 0., 0.}); // east
    test({1.25, 1.25, 0}, {-10., 0., 0.});  // west
}

// Tests the reflective boundary condition using a single particle in a cell container.
// Upon entering a halo cell, the velocity is inverted and the mirrored position of the particle is computed.
// See the report and the presentation slides for more details.
TEST(BoundaryConditionTests, Reflective2D) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{10., 10., 1.}, conditions, 1., pc, 2};
        calculateX_LC(pc, delta_t, 0.0, &c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_EQ(pc[0].getCellIndex(), expectedIndex);
        EXPECT_TRUE(pc[0].isActive());
    };
    test({10.75, 10.75, 0}, {0., 10., 0.}, {10.75, 10.75, 0}, {0, -10., 0.}, 130); // north
    test({1.25, 1.25, 0}, {0., -10., 0.}, {1.25, 1.25, 0}, {0, 10., 0.}, 13);      // south
    test({10.75, 10.75, 0}, {10., 0., 0.}, {10.75, 10.75, 0}, {-10, 0., 0.}, 130); // east
    test({1.25, 1.25, 0}, {-10., 0., 0.}, {1.25, 1.25, 0}, {10, 0., 0.}, 13);      // west
}

// Tests handling reflective corners.
// The particle should be mirrored and have it's velocity flipped in the X and Y directions.
TEST(BoundaryConditionTests, ReflectiveCorners2D) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex, bool expectedActive) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{10., 10., 1.}, conditions, 1., pc, 2};
        calculateX_LC(pc, delta_t, 0.0, &c);

        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), expectedActive ? 1 : 0);
        EXPECT_EQ(pc[0].getCellIndex(), expectedIndex);
        if (expectedActive) {
            EXPECT_EQ(pc[0].getX(), expectedPos);
            EXPECT_EQ(pc[0].getV(), expectedVel);
        }
        EXPECT_EQ(pc[0].isActive(), expectedActive);
    };

    test({10.8, 10.8, 0}, {5., 10., 0.}, {10.95, 10.7, 0}, {-5., -10., 0}, 130, true); // NE: N first -> reflect
    test({1.2, 1.2, 0}, {-10., -5., 0.}, {1.3, 1.05, 0}, {10., 5., 0}, 13, true);      // SW: W first -> reflect
    test({1.2, 10.8, 0}, {-5., 10., 0.}, {1.05, 10.7, 0}, {5., -10., 0}, 121, true);   // NW: N first -> reflect
    test({1.2, 10.8, 0}, {-10., 5., 0.}, {1.3, 10.95, 0}, {10., -5., 0}, 121, true);   // NW: W first -> reflect
}

// Tests handling basic periodic boundaries.
TEST(BoundaryConditionTests, Periodic2D) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{5., 5., 1.}, conditions, 1., pc, 2};
        calculateX_LC(pc, delta_t, 0.0, &c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_EQ(pc[0].getCellIndex(), expectedIndex);
        EXPECT_TRUE(pc[0].isActive());
    };
    test({1.5, 5.5, 0}, {0., 20., 0.}, {1.5, 1.5, 0}, {0., 20., 0.}, 8);   // north
    test({1.5, 1.5, 0}, {0., -20., 0.}, {1.5, 5.5, 0}, {0, -20., 0.}, 36); // south
    test({1.5, 1.5, 0}, {-20., 0., 0.}, {5.5, 1.5, 0}, {-20, 0., 0.}, 12); // west
    test({5.5, 1.5, 0}, {20., 0., 0.}, {1.5, 1.5, 0}, {20, 0., 0.}, 8);    // east

    // this is the test for corners (double mirroring)
    test({6.75, 1.75, 0.}, {0., -20., 0.}, {1.75, 5.75, 0.}, {0., -20., 0.}, 36);
}

// Tests mirroring ghost particles when used with periodic boundaries.
TEST(BoundaryConditionTests, PeriodicMirroring2D) {
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedGhostIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{5., 5., 1.}, conditions, 1., pc, 2};
        mirrorGhostParticles(&c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_TRUE(pc[0].isActive());

        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex].getParticles().begin(),
                                c.getCells()[expectedGhostIndex].getParticles().end()),
                  1);
    };
    test({2.5, 5.5, 0}, {0., 0., 0.}, {2.5, 5.5, 0}, {0., 0., 0.}, 2);  // north
    test({2.5, 1.5, 0}, {0., 0., 0.}, {2.5, 1.5, 0}, {0., 0., 0.}, 44); // south
    test({1.5, 2.5, 0}, {0., 0., 0.}, {1.5, 2.5, 0}, {0., 0., 0.}, 20); // west
    test({5.5, 2.5, 0}, {0., 0., 0.}, {5.5, 2.5, 0}, {0., 0., 0.}, 14); // east
}

// Test handling periodic corner halo cells.
TEST(BoundaryConditionTests, PeriodicMirroringCorner2D) {
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    const std::array<double, 3> expectedGhostIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{5., 5., 1.}, conditions, 1., pc, 2};
        mirrorGhostParticles(&c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(c.activeSize(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_TRUE(pc[0].isActive());

        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex[0]].getParticles().begin(),
                                c.getCells()[expectedGhostIndex[0]].getParticles().end()),
                  1);
        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex[1]].getParticles().begin(),
                                c.getCells()[expectedGhostIndex[1]].getParticles().end()),
                  1);
        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex[2]].getParticles().begin(),
                                c.getCells()[expectedGhostIndex[2]].getParticles().end()),
                  1);
    };
    test({1.5, 1.5, 0}, {0., 0., 0.}, {1.5, 1.5, 0}, {0., 0., 0.}, {43, 13, 48}); // sw corner
}

TEST(BoundaryConditionTests, MixedReflectivePeriodicCorner) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::PERIODIC,   BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC,   BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{5., 5., 1.}, conditions, 1., pc, 2};
        calculateX_LC(pc, delta_t, 0.0, &c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_EQ(pc[0].getCellIndex(), expectedIndex);
        EXPECT_TRUE(pc[0].isActive());
    };
    test({5.75, 5.75, 0.}, {20., 20., 0.}, {1.75, 5.25, 0.}, {20., -20., 0.}, 36);   // ne corner
    test({5.25, 1.25, 0.}, {20., -20., 0.}, {1.25, 1.75, 0.}, {20., 20., 0.}, 8);    // se corner
    test({1.25, 1.25, 0.}, {-20., -20., 0.}, {5.25, 1.75, 0.}, {-20., 20., 0.}, 12); // sw corner
    test({1.25, 5.75, 0.}, {-20., 20., 0.}, {5.25, 5.25, 0.}, {-20., -20., 0.}, 40); // nw corner
}

TEST(BoundaryConditionTests, Periodic3D) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{2., 2., 2.}, conditions, 1., pc, 3};
        calculateX_LC(pc, delta_t, 0.0, &c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_EQ(pc[0].getCellIndex(), expectedIndex);
        EXPECT_TRUE(pc[0].isActive());
    };

    test({2.5, 1.5, 1.5}, {20., 0., 0.}, {1.5, 1.5, 1.5}, {20., 0., 0.}, 21); // X
    test({1.5, 2.5, 1.5}, {0., 20., 0.}, {1.5, 1.5, 1.5}, {0, 20., 0.}, 21);  // Y
    test({1.5, 1.5, 2.5}, {0., 0., 20.}, {1.5, 1.5, 1.5}, {0, 0., 20.}, 21);  // Z

    // this is the test for corners (double mirroring)
    test({2.75, 1.5, 1.75}, {20., 0., -20.}, {1.75, 1.5, 2.75}, {20., 0., -20.}, 37);
    // triple mirroring
    test({2.75, 1.5, 1.75}, {10., -20., -20.}, {1.25, 2.5, 2.75}, {10., -20., -20.}, 41);
}

TEST(BoundaryConditionTests, PeriodicMirroringTriple3D) {
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedGhostIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{2., 2., 2.}, conditions, 1., pc, 3};
        mirrorGhostParticles(&c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_TRUE(pc[0].isActive());

        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex].getParticles().begin(),
                                c.getCells()[expectedGhostIndex].getParticles().end()),
                  1);
    };
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 20); // X
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 30); // Y
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 54); // Z
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 28); // XY
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 52); // XZ
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 62); // YZ
    test({2.5, 1.5, 1.5}, {0., 0., 0.}, {2.5, 1.5, 1.5}, {0., 0., 0.}, 60); // XYZ
}

TEST(BoundaryConditionTests, PeriodicMirroring3D) {
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC,
                                                BoundaryCondition::PERIODIC, BoundaryCondition::PERIODIC};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedGhostIndex) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{3., 3., 3.}, conditions, 1., pc, 3};
        mirrorGhostParticles(&c);
        EXPECT_EQ(c.size(), 1);
        EXPECT_EQ(pc[0].getX(), expectedPos);
        EXPECT_EQ(pc[0].getV(), expectedVel);
        EXPECT_TRUE(pc[0].isActive());

        EXPECT_EQ(std::distance(c.getCells()[expectedGhostIndex].getParticles().begin(),
                                c.getCells()[expectedGhostIndex].getParticles().end()),
                  1);
    };

    // double boundary
    test({3.5, 1.5, 2.5}, {0., 0., 0.}, {3.5, 1.5, 2.5}, {0., 0., 0.}, 55); // X
    test({3.5, 1.5, 2.5}, {0., 0., 0.}, {3.5, 1.5, 2.5}, {0., 0., 0.}, 73); // Y
    test({3.5, 1.5, 2.5}, {0., 0., 0.}, {3.5, 1.5, 2.5}, {0., 0., 0.}, 70); // XY

    // single boundary
    test({3.5, 2.5, 2.5}, {0., 0., 0.}, {3.5, 2.5, 2.5}, {0., 0., 0.}, 60); // X
}
