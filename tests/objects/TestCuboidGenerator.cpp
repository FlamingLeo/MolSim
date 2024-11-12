#include "objects/CuboidGenerator.h"
#include <filesystem>
#include <gtest/gtest.h>

class CuboidGeneratorTests : public ::testing::Test {
  private:
    std::string getTargetPath(const std::string &fullPath, const std::string &targetDir) {
        size_t pos = fullPath.find(targetDir);
        if (pos != std::string::npos) {
            return fullPath.substr(0, pos + targetDir.length());
        }
        return "";
    }

  protected:
    std::string targetPath;

    void SetUp() override {
        std::filesystem::path cwd = std::filesystem::current_path();
        targetPath = getTargetPath(cwd, "MolSim");
        if (targetPath.empty())
            GTEST_SKIP() << "Project root directory not found, skipping tests...";
        targetPath += "/tests/files";
        if (!(std::filesystem::exists(targetPath)))
            GTEST_SKIP() << "Test files not found, skipping tests...";
    }
};

// Test attempting to generate cuboids from an invalid file source.
TEST_F(CuboidGeneratorTests, InvalidFile) {
    ParticleContainer pc;
    CuboidGenerator cg0("", pc);
    CuboidGenerator cg1("/", pc);
    CuboidGenerator cg2("/dev/null/foo", pc);
    EXPECT_DEATH({ cg0.generateCuboids(); }, "");
    EXPECT_DEATH({ cg1.generateCuboids(); }, "");
    EXPECT_DEATH({ cg2.generateCuboids(); }, "");
}

// Test attempting to generate cuboids in a non-empty particle container.
// Will only halt on debug builds; otherwise, will execute as normal.
TEST_F(CuboidGeneratorTests, NonEmptyParticleContainer) {
    ParticleContainer pc;
    pc.addParticle({1., 2., 3.}, {4., 5., 6.}, 7.);
    CuboidGenerator cg(targetPath + "/testCuboidInput.txt", pc);

    EXPECT_DEBUG_DEATH({ cg.generateCuboids(); }, "");
}

// Test generating cuboids and intializing them into a ParticleContainer from a given source file.
TEST_F(CuboidGeneratorTests, GenerateCuboids) {
    ParticleContainer pc;
    CuboidGenerator cg(targetPath + "/testCuboidInput.txt", pc);
    cg.generateCuboids();

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