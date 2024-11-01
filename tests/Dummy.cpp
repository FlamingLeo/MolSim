// DUMMY TEST FILE: http://google.github.io/googletest/quickstart-cmake.html
#include "objects/ParticleContainer.h" // CHECK IF LINKING WORKS
#include <gtest/gtest.h>

TEST(HelloTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}
