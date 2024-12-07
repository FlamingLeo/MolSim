#include "Cuboid.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Cuboid::Cuboid(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<size_t, 3> &N,
               const std::array<double, 3> &v, double h, double m, int type, double epsilon, double sigma)
    : x{x}, N{N}, h{h}, m{m}, type{type}, epsilon{epsilon}, sigma{sigma}, v{v}, mean_velocity{0.1},
      particles{particles} {
    SPDLOG_TRACE("Generated Cuboid (simple constructor) - x: {}, N: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 ArrayUtils::to_string(x), ArrayUtils::to_string(N), h, m, ArrayUtils::to_string(v), mean_velocity);
}

void Cuboid::initializeParticles(size_t dimensions) {
    SPDLOG_TRACE("Initializing Particles for Cuboid {}...", this->toString());
    std::array<double, 3> xyz;
    for (size_t i = 0; i < N[2]; i++) {
        for (size_t j = 0; j < N[1]; j++) {
            for (size_t k = 0; k < N[0]; k++) {
                xyz = {x[0] + k * h, x[1] + j * h, x[2] + i * h};
                v = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, dimensions),
                                                  std::plus<>());
                particles.addParticle(xyz, v, m, type, epsilon, sigma);
            }
        }
    }
}

const std::array<double, 3> &Cuboid::getX() const { return x; }
const std::array<size_t, 3> &Cuboid::getN() const { return N; }
const double Cuboid::getH() const { return h; }
const double Cuboid::getM() const { return m; }
const std::array<double, 3> &Cuboid::getV() const { return v; }
const double Cuboid::getMeanVelocity() const { return mean_velocity; }
const ParticleContainer &Cuboid::getParticles() const { return particles; }

bool Cuboid::operator==(const Cuboid &other) const {
    return (x == other.x) && (N == other.N) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles);
}
bool Cuboid::operator!=(const Cuboid &other) const { return !(*this == other); }

std::string Cuboid::toString() const {
    std::stringstream stream;
    stream << "{ x: " << x << ", N: " << N << ", v: " << v << ", h: " << h << ", m: " << m
           << ", particles: " << particles.toString() << " }";
    return stream.str();
}
