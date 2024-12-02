#include "objects/CellContainer.h"
#include "objects/Particle.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>

class CellContainerTest : public ::testing::Test {
  protected:
    std::array<double, 3> domainSize{10.0, 10.0, 1.0};
    double cutoff{2.0};
    std::array<BoundaryCondition, 6> conditions{BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                                                BoundaryCondition::OUTFLOW,    BoundaryCondition::OUTFLOW,
                                                BoundaryCondition::OUTFLOW,    BoundaryCondition::OUTFLOW};
    ParticleContainer particles;
    CellContainer container{domainSize, conditions, cutoff, particles, 2};

    void SetUp() override {
        particles.addParticle(Particle({1.0, 1.0, 0.0},
                                       {
                                           0.,
                                           0.,
                                           0.,
                                       },
                                       1));
        particles.addParticle(Particle({5.0, 5.0, 0.0},
                                       {
                                           0.,
                                           0.,
                                           0.,
                                       },
                                       1));
        particles.addParticle(Particle({8.0, 8.0, 0.0},
                                       {
                                           0.,
                                           0.,
                                           0.,
                                       },
                                       1));
        for (auto &p : particles) {
            container.addParticle(p);
        }
    }
};

// Test attempting to initialize a cell container with less than 2 or more than 3 dimensions.
// The program should terminate.
TEST_F(CellContainerTest, InitializeInvalidDimensions) {
    EXPECT_DEATH(CellContainer(domainSize, conditions, cutoff, particles, 1), "");
    EXPECT_DEATH(CellContainer(domainSize, conditions, cutoff, particles, 4), "");
}

// Test attempting to initialize a cell container using partially or fully uninitialized values.
// The program should terminate.
TEST_F(CellContainerTest, InitializeUninitializedValues) {
    EXPECT_DEATH(CellContainer({INF, 10.0, 10.0}, conditions, cutoff, particles, 3), "");
    EXPECT_DEATH(CellContainer(domainSize, conditions, INF, particles, 3), "");
}

// Test adding a particle to a cell container.
TEST_F(CellContainerTest, AddParticle) {
    Particle p1({2.0, 2.0, 0.0},
                {
                    0.,
                    0.,
                    0.,
                },
                1);
    ASSERT_TRUE(container.addParticle(p1));
    EXPECT_EQ(p1.getCellIndex(), 8);
    EXPECT_EQ(p1.getCellIndex(), container.getCellIndex(p1.getX()));
}

// Test attempting to add a particle outside the domain.
TEST_F(CellContainerTest, ParticleAdditionOutsideDomain) {
    Particle p1({-1.0, -1.0, 0.0},
                {
                    0.,
                    0.,
                    0.,
                },
                1);
    ASSERT_FALSE(container.addParticle(p1));
    EXPECT_EQ(p1.getCellIndex(), -1);
}

// Test moving a particle to its correct cell.
TEST_F(CellContainerTest, ParticleMovement) {
    Particle &p = particles[0];
    ASSERT_TRUE(container.moveParticle(p));
    EXPECT_EQ(p.getCellIndex(), 0);
    EXPECT_EQ(p.getCellIndex(), container.getCellIndex(p.getX()));
}

// Test removing a particle from the cell container (and thereby it's corresponding cell).
TEST_F(CellContainerTest, ParticleDeletion) {
    Particle &p = particles[1];
    int cellIndex = p.getCellIndex();
    container.deleteParticle(p);
    EXPECT_EQ(p.getCellIndex(), -1);
    EXPECT_TRUE(container[cellIndex].getParticles().empty());
}

// Test getting the neighbor cells of a cell from a given position.
TEST_F(CellContainerTest, GetNeighbors) {
    int cellIndex = container.getCellIndex({5.0, 5.0, 0.0});
    auto neighbors = container.getNeighbors(cellIndex);
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), cellIndex) != neighbors.end());
}

// Test iterating over boundary particles.
TEST_F(CellContainerTest, BoundaryIterator) {
    Particle p1({0.5, 0.5, 0.0},
                {
                    0.,
                    0.,
                    0.,
                },
                1);
    Particle p2({9.5, 9.5, 0.0},
                {
                    0.,
                    0.,
                    0.,
                },
                1);
    Particle p3({5.0, 5.0, 0.0},
                {
                    0.,
                    0.,
                    0.,
                },
                1);

    container.addParticle(p1);
    container.addParticle(p2);
    container.addParticle(p3);

    // check boundary particles
    auto it = container.boundaryBegin();
    auto end = container.boundaryEnd();
    for (; it != end; ++it) {
        Particle &p = *it;
        EXPECT_EQ(container[p.getCellIndex()].getType(), CellType::BORDER);
    }
}

// Test iterating over boundary cell particles in an empty container.
TEST_F(CellContainerTest, BoundaryIteratorEmpty) {
    ParticleContainer pc;
    CellContainer empty({10, 10, 1}, conditions, 1.0, pc, 2);
    auto it = empty.boundaryBegin();
    auto end = empty.boundaryEnd();
    for (; it != end; ++it) {
        GTEST_FAIL() << "Should not reach this!";
    }
}

// Test iterating over boundary cell particles in a 1-element container, where the element is a boundary cell particle.
TEST_F(CellContainerTest, BoundaryIteratorOneElement) {
    ParticleContainer pc;
    Particle p({10.0, 10.0, 0.0},
               {
                   0.,
                   0.,
                   0.,
               },
               1);
    pc.addParticle(p);
    CellContainer oneEl({10, 10, 1}, conditions, 1.0, pc, 2);

    auto it = oneEl.boundaryBegin();
    auto end = oneEl.boundaryEnd();
    ASSERT_NE(it, end);
    EXPECT_EQ(*it, p);
    ++it;
    EXPECT_EQ(it, end);
}

// Test iterating over halo cell particles.
TEST_F(CellContainerTest, HaloIterator) {
    // add some (more) halo particles...
    Particle p1 = {{2., 1., 0.},
                   {
                       0.,
                       0.,
                       0.,
                   },
                   1};
    Particle p2 = {{3., 1., 0.},
                   {
                       0.,
                       0.,
                       0.,
                   },
                   1};
    Particle p3 = {{4., 1., 0.},
                   {
                       0.,
                       0.,
                       0.,
                   },
                   1};

    container.addParticle(p1);
    container.addParticle(p2);
    container.addParticle(p3);

    // check halo particles
    auto it = container.haloBegin();
    auto end = container.haloEnd();
    for (; it != end; ++it) {
        Particle &p = *it;
        EXPECT_EQ(container[p.getCellIndex()].getType(), CellType::HALO);
    }
}

// Test iterating over halo cell particles in an empty container.
TEST_F(CellContainerTest, HaloIteratorEmpty) {
    ParticleContainer pc;
    CellContainer empty({10, 10, 1}, conditions, 1.0, pc, 2);
    auto it = empty.haloBegin();
    auto end = empty.haloEnd();
    for (; it != end; ++it) {
        GTEST_FAIL() << "Should not reach this!";
    }
}

// Test iterating over halo cell particles in a one-element container, where the element is a halo cell particle.
TEST_F(CellContainerTest, HaloIteratorOneElement) {
    ParticleContainer pc;
    Particle p({0.5, 0.5, 0.0},
               {
                   0.,
                   0.,
                   0.,
               },
               1);
    pc.addParticle(p);
    CellContainer oneEl({10, 10, 1}, conditions, 1.0, pc, 2);

    auto it = oneEl.haloBegin();
    auto end = oneEl.haloEnd();
    ASSERT_NE(it, end);
    EXPECT_EQ(*it, p);
    ++it;
    EXPECT_EQ(it, end);
}

// Test iterating over the entire cell container.
TEST_F(CellContainerTest, CellIterator) {
    size_t total = 0;
    for (auto &cell : container) {
        for (auto *p : cell.getParticles()) {
            total++;
        }
    }
    EXPECT_EQ(total, 3);
}

// Test iterating over an empty cell container.
TEST_F(CellContainerTest, CellIteratorEmpty) {
    ParticleContainer pc;
    CellContainer empty({10, 10, 1}, conditions, 1.0, pc, 2);
    for (auto &c : empty) {
        EXPECT_TRUE(c.getParticles().empty());
    }
}

// Test getting the index of the opposite neighbor cell.
TEST_F(CellContainerTest, GetOppositeNeighbor) {
    int cellIndex = 121;
    std::vector<HaloLocation> directions = {HaloLocation::NORTH};
    int expectedIndex = 114;
    int result = container.getOppositeNeighbor(cellIndex, directions);
    EXPECT_EQ(result, expectedIndex);

    directions = {HaloLocation::SOUTH};
    expectedIndex = 128;
    result = container.getOppositeNeighbor(cellIndex, directions);
    EXPECT_EQ(result, expectedIndex);

    directions = {HaloLocation::EAST};
    expectedIndex = 120;
    result = container.getOppositeNeighbor(cellIndex, directions);
    EXPECT_EQ(result, expectedIndex);

    directions = {HaloLocation::WEST};
    expectedIndex = 122;
    result = container.getOppositeNeighbor(cellIndex, directions);
    EXPECT_EQ(result, expectedIndex);

    directions = {HaloLocation::NORTH, HaloLocation::EAST};
    expectedIndex = 113;
    result = container.getOppositeNeighbor(cellIndex, directions);
    EXPECT_EQ(result, expectedIndex);
}

// Test getting the mirrored position of a particle from two cells.
TEST_F(CellContainerTest, GetMirrorPosition) {
    std::array<double, 3> position = {2.0, 2.0, 0.0};
    Cell fromCell({1.0, 1.0, 1.0}, {1.5, 1.5, 0.0}, CellType::INNER, 121, {});
    Cell toCell({1.0, 1.0, 1.0}, {1.5, 3.5, 0.0}, CellType::INNER, 133, {});
    std::array<double, 3> expectedPosition = {2.0, 4.0, 0.0};
    std::array<double, 3> result = container.getMirrorPosition(position, fromCell, toCell, 1);
    EXPECT_EQ(result, expectedPosition);

    toCell = Cell({1.0, 1.0, 1.0}, {1.5, -1.5, 0.0}, CellType::INNER, 1, {});
    expectedPosition = {2.0, -1.0, 0.0};
    result = container.getMirrorPosition(position, fromCell, toCell, 1);
    EXPECT_EQ(result, expectedPosition);

    toCell = Cell({1.0, 1.0, 1.0}, {3.5, 1.5, 0.0}, CellType::INNER, 122, {});
    expectedPosition = {4.0, 2.0, 0.0};
    result = container.getMirrorPosition(position, fromCell, toCell, 0);
    EXPECT_EQ(result, expectedPosition);

    toCell = Cell({1.0, 1.0, 1.0}, {-0.5, 1.5, 0.0}, CellType::INNER, 120, {});
    expectedPosition = {0.0, 2.0, 0.0};
    result = container.getMirrorPosition(position, fromCell, toCell, 0);
    EXPECT_EQ(result, expectedPosition);

    toCell = Cell({1.0, 1.0, 1.0}, {1.5, 1.5, 2.5}, CellType::INNER, 122, {});
    expectedPosition = {2.0, 2.0, 3.5};
    result = container.getMirrorPosition(position, fromCell, toCell, 2);
    EXPECT_EQ(result, expectedPosition);

    toCell = Cell({1.0, 1.0, 1.0}, {1.5, 1.5, -0.5}, CellType::INNER, 120, {});
    expectedPosition = {2.0, 2.0, 0.5};
    result = container.getMirrorPosition(position, fromCell, toCell, 2);
    EXPECT_EQ(result, expectedPosition);
}
