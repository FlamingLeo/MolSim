/**
 * @file RNG.h
 * @brief Utility functions for generating random numbers.
 * @date 2024-11-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <random>

/// @brief Namespace defining utility functions for generating random numbers.
namespace RNG {
/**
 * @brief Returns a random number between the start and end values, inclusive.
 *
 * This random number generator uses the Mersenne Twister and the platform-specific random number generator as an
 * initializer value.
 *
 * @param start The start of the random number range.
 * @param end The end of the random number range.
 * @return A random number in the specified range.
 */
static inline int randMersenne(int start, int end) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
    return dist(rng);
}
} // namespace RNG
