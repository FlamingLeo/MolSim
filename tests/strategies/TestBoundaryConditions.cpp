#include "strategies/PositionCalculation.h"
#include <gtest/gtest.h>

// Tests the outflow boundary condition using a single particle in a cell container.
// Upon entering a halo cell, the particle is removed from the cell container and is marked inactive.
TEST(BoundaryConditionTests, Outflow) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW, BoundaryCondition::OUTFLOW};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{10., 10., 1.}, conditions, 1., pc};
        calculateX_LC(pc, delta_t, &c);
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
TEST(BoundaryConditionTests, Reflective) {
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
        CellContainer c{{10., 10., 1.}, conditions, 1., pc};
        calculateX_LC(pc, delta_t, &c);
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

// Tests choosing the correct boundary condition based on which edge is hit first.
// See the report and the presentation slides for more details.
TEST(BoundaryConditionTests, MixedCorners) {
    constexpr double delta_t = 0.05;
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::REFLECTIVE, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::REFLECTIVE, BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW,    BoundaryCondition::OUTFLOW};

    auto test = [&](const std::array<double, 3> &position, const std::array<double, 3> &velocity,
                    const std::array<double, 3> &expectedPos, const std::array<double, 3> &expectedVel,
                    int expectedIndex, bool expectedActive) {
        ParticleContainer pc;
        Particle p{position, velocity, 1, 1};
        pc.addParticle(p);
        CellContainer c{{10., 10., 1.}, conditions, 1., pc};
        calculateX_LC(pc, delta_t, &c);

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
    test({10.8, 10.8, 0}, {10., 5., 0.}, {}, {}, -1, false);                           // NE: E first -> deleted
    test({10.8, 1.2, 0}, {5., -10., 0.}, {}, {}, -1, false);                           // SE: S first -> deleted
    test({10.8, 1.2, 0}, {10., -5., 0.}, {}, {}, -1, false);                           // SE: E first -> deleted
    test({1.2, 1.2, 0}, {-5., -10., 0.}, {}, {}, -1, false);                           // SW: S first -> deleted
    test({1.2, 1.2, 0}, {-10., -5., 0.}, {1.3, 1.05, 0}, {10., 5., 0}, 13, true);      // SW: W first -> reflect
    test({1.2, 10.8, 0}, {-5., 10., 0.}, {1.05, 10.7, 0}, {5., -10., 0}, 121, true);   // NW: N first -> reflect
    test({1.2, 10.8, 0}, {-10., 5., 0.}, {1.3, 10.95, 0}, {10., -5., 0}, 121, true);   // NW: W first -> reflect
}
