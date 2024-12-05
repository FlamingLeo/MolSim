#include "io/input/FileReader.h"
#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include "utils/PathUtils.h"
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
  protected:
    std::string targetPath;

    void SetUp() override {
        std::string failReason = "";
        if (!PathUtils::setupFileTests(targetPath, failReason))
            GTEST_SKIP() << failReason;
    }
};

// Test opening an invalid file.
TEST_F(FileReaderTests, OpenFileInvalid) {
    EXPECT_DEATH({ FileReader f(targetPath); }, "");      // directory
    EXPECT_DEATH({ FileReader f("/dev/null/foo"); }, ""); // non-existent file
}