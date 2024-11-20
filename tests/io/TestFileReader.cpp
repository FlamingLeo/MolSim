#include "io/input/FileReader.h"
#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#define EXPECT_NO_EXIT(cmd, msg)                                                                                       \
    do {                                                                                                               \
        EXPECT_EXIT(                                                                                                   \
            {                                                                                                          \
                cmd;                                                                                                   \
                exit(0);                                                                                               \
            },                                                                                                         \
            ::testing::ExitedWithCode(0), msg);                                                                        \
    } while (0)

class FileReaderTests : public ::testing::Test {
  private:
    std::string getTargetPath(const std::string &fullPath, const std::string &targetDir) {
        size_t pos = fullPath.rfind(targetDir);
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

// Test opening a valid file.
TEST_F(FileReaderTests, OpenFileValid) {
    EXPECT_NO_EXIT({ FileReader f(targetPath + "/testParticleInput.txt"); }, "");
}

// Test opening an invalid file.
TEST_F(FileReaderTests, OpenFileInvalid) {
    EXPECT_DEATH({ FileReader f(targetPath); }, "");      // directory
    EXPECT_DEATH({ FileReader f("/dev/null/foo"); }, ""); // non-existent file
}

// Test reading Particle data from an empty file.
TEST_F(FileReaderTests, ReadParticleDataEmpty) {
    ParticleContainer pc;
    std::string tmpFileName = std::tmpnam(nullptr);
    std::ofstream _output(tmpFileName);
    FileReader fr(tmpFileName);
    EXPECT_DEATH({ fr.readParticles(&pc); }, "");
}

// Test reading Particle data into a ParticleContainer.
TEST_F(FileReaderTests, ReadParticleDataNonEmpty) {
    ParticleContainer pc;
    FileReader fr(targetPath + "/testParticleInput.txt");
    fr.readParticles(&pc);

    ASSERT_EQ(pc.size(), 2);

    constexpr std::array f = {0.0, 0.0, 0.0};
    constexpr std::array oldF = {0.0, 0.0, 0.0};
    constexpr int type = 0;

    constexpr std::array x1 = {0.0, 0.0, 0.0};
    constexpr std::array v1 = {0.0, 0.0, 0.0};
    constexpr double m1 = 1.0;

    EXPECT_EQ(pc[0].getX(), x1);
    EXPECT_EQ(pc[0].getV(), v1);
    EXPECT_EQ(pc[0].getF(), f);
    EXPECT_EQ(pc[0].getOldF(), oldF);
    EXPECT_EQ(pc[0].getM(), m1);
    EXPECT_EQ(pc[0].getType(), type);

    constexpr std::array x2 = {0.0, 1.0, 0.0};
    constexpr std::array v2 = {-1.0, 0.0, 0.0};
    constexpr double m2 = 3.0e-6;

    EXPECT_EQ(pc[1].getX(), x2);
    EXPECT_EQ(pc[1].getV(), v2);
    EXPECT_EQ(pc[1].getF(), f);
    EXPECT_EQ(pc[1].getOldF(), oldF);
    EXPECT_EQ(pc[1].getM(), m2);
    EXPECT_EQ(pc[1].getType(), type);
}

// Test reading Cuboid data from an empty file.
TEST_F(FileReaderTests, ReadCuboidDataEmpty) {
    ParticleContainer pc;
    std::string tmpFileName = std::tmpnam(nullptr);
    std::ofstream _output(tmpFileName);
    FileReader fr(tmpFileName);
    EXPECT_DEATH({ fr.readCuboids(pc); }, "");
}

// Test reading Cuboid data into a ParticleContainer.
TEST_F(FileReaderTests, ReadCuboidDataNonEmpty) {
    ParticleContainer pc;
    FileReader fr(targetPath + "/testCuboidInput.txt");
    fr.readCuboids(pc);

    ASSERT_EQ(pc.size(), 6);

    constexpr std::array x0 = {0., 0., 0., 0., 0., 0.};
    constexpr std::array x1 = {0., 1.1225, 0., 1.1225, 0., 1.1225};
    constexpr std::array x2 = {0., 0., 1.1225, 1.1225, 2.245, 2.245};
    constexpr std::array f = {0., 0., 0.};
    constexpr std::array oldF = {0., 0., 0.};
    constexpr double m = 1.0;
    constexpr double eps = 0.00001;

    for (size_t i = 0; i < 6; ++i) {
        EXPECT_NEAR(pc[i].getX()[0], x0[i], eps);
        EXPECT_NEAR(pc[i].getX()[1], x1[i], eps);
        EXPECT_NEAR(pc[i].getX()[2], x2[i], eps);
        EXPECT_EQ(pc[i].getF(), f);
        EXPECT_EQ(pc[i].getOldF(), oldF);
        EXPECT_EQ(pc[i].getM(), m);
    }
}