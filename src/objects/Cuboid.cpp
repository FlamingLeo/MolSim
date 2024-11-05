//
// Created by marag on 11/5/2024.
//

#include "Cuboid.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "utils/StringUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Cuboid::Cuboid(const ParticleContainer &particles, const std::array<double, 3> &position,
               const std::array<int, 3> &size, const std::array<double, 3> &v, double h, double m)
    : position{position}, size{size}, h{h}, m{m}, v{v}, mean_velocity{0.1}, particles{particles} {
    SPDLOG_TRACE("Generated Cuboid (simple constructor) - position: {}, size: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 StringUtils::fromArray(position), StringUtils::fromArray(size), h, m, StringUtils::fromArray(v),
                 mean_velocity);
}

ParticleContainer Cuboid::initializeParticles() {
    v = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 3), std::plus<>());
    std::array<double, 3> xyz;
    for (int i = 0; i < size[2]; i++) {
        for (int j = 0; j < size[1]; j++) {
            for (int k = 0; k < size[0]; k++) {
                xyz = {position[0] + k * h, position[1] + j * h, position[2] + i * h};
                particles.addParticle(xyz, v, m);
            }
        }
    }
}