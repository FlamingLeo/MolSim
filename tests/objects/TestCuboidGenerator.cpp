#include "objects/CuboidGenerator.h"
#include "utils/PathUtils.h"
#include <filesystem>
#include <gtest/gtest.h>

class CuboidGeneratorTests : public ::testing::Test {
  protected:
    std::string targetPath;

    void SetUp() override {
        std::string failReason = "";
        if (!PathUtils::setupFileTests(targetPath, failReason))
            GTEST_SKIP() << failReason;
    }
};

// Test attempting to generate cuboids from an invalid file source.
TEST_F(CuboidGeneratorTests, InvalidFile) {
    ParticleContainer pc;
    EXPECT_DEATH(
        {
            CuboidGenerator cg0("", pc);
            cg0.generateCuboids();
        },
        "");
    EXPECT_DEATH({ CuboidGenerator cg1("/", pc); }, "");
    EXPECT_DEATH({ CuboidGenerator cg2("/dev/null/foo", pc); }, "");
}

// Test generating cuboids and intializing them into a ParticleContainer from a given source file.
TEST_F(CuboidGeneratorTests, GenerateCuboids) {
    ParticleContainer pc;
    CuboidGenerator cg(targetPath + "/testCuboidInput.txt", pc);

    ASSERT_EQ(pc.size(), 6);

    constexpr std::array<std::array<double, 6>, 3> expectedX = {
        {{0., 0., 0., 0., 0., 0.}, {0., 1.1225, 0., 1.1225, 0., 1.1225}, {0., 0., 1.1225, 1.1225, 2.245, 2.245}}};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double m = 1.0;
    constexpr double eps = 0.00001;

    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_NEAR(pc[i].getX()[j], expectedX[j][i], eps);
        }
        EXPECT_EQ(pc[i].getF(), f);
        EXPECT_EQ(pc[i].getOldF(), oldF);
        EXPECT_EQ(pc[i].getM(), m);
    }
}