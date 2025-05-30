/**
 * @file MaxwellBoltzmannDistribution.h
 * @author F. Gratl
 * @brief Utility function(s) for working with Maxwell-Boltzmann distributions.
 * @date 2019-12-13
 *
 * @copyright Copyright (c) 2019 - 2024
 *
 */

#pragma once

#include <array>
#include <random>

/**
 * @brief Generate a random velocity vector according to the Maxwell-Boltzmann
 * distribution, with a given average velocity.
 *
 * @param averageVelocity The average velocity of the brownian motion for the
 * system.
 * @param dimensions Number of dimensions for which the velocity vector shall be
 * generated. Set this to 2 or 3.
 * @return Array containing the generated velocity vector.
 */
inline std::array<double, 3> maxwellBoltzmannDistributedVelocity(double averageVelocity, size_t dimensions) {
    // we use a constant seed for repeatability.
    // random engine needs static lifetime otherwise it would be recreated for
    // every call.
    static std::default_random_engine randomEngine(42);

    // when adding independent normally distributed values to all velocity
    // components the velocity change is maxwell boltzmann distributed
    std::normal_distribution<double> normalDistribution{0, 1};
    std::array<double, 3> randomVelocity{};
    for (size_t i = 0; i < dimensions; ++i) {
        randomVelocity[i] = averageVelocity * normalDistribution(randomEngine);
    }
    return randomVelocity;
}
