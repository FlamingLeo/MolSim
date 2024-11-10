#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <gtest/gtest.h>

// Test initializing the cuboid particles (i.e. if the positions are correct, if the force effective is 0 and if the
// masses of all the particles are the same).
TEST(CuboidTests, InitializeCuboidParticles) {
    ParticleContainer pc;
    Cuboid c{pc, {1., 2., 3.}, {1, 2, 3}, {1., 2., 3.}, 1., 1.};
    c.initializeParticles();

    constexpr std::array x0 = {1., 1., 1., 1., 1., 1.};
    constexpr std::array x1 = {2., 3., 2., 3., 2., 3.};
    constexpr std::array x2 = {3., 3., 4., 4., 5., 5.};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double eps = 0.00001;

    ASSERT_EQ(c.getParticles().size(), 6);

    for (size_t i = 0; i < 6; ++i) {
        Particle &p = c.getParticles()[i];

        std::cout << p.getV()[0] << ", " << p.getV()[1] << ", " << p.getV()[2] << "\n";

        EXPECT_EQ(p.getX()[0], x0[i]);
        EXPECT_EQ(p.getX()[1], x1[i]);
        EXPECT_EQ(p.getX()[2], x2[i]);
        EXPECT_EQ(p.getF(), f);
        EXPECT_EQ(p.getOldF(), oldF);
        EXPECT_EQ(p.getM(), 1.0);
    }
}
