#include "Disc.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Disc::Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r, const std::array<double, 3> &v,
           double h, double m)
    : x{x}, r{r}, h{h}, m{m}, v{v}, mean_velocity{0.1}, particles{particles} {
    SPDLOG_TRACE("Generated Disc (simple constructor) - x: {}, r: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 ArrayUtils::to_string(x), r, h, m, ArrayUtils::to_string(v), mean_velocity);
}

void Disc::initializeDisc() {
    SPDLOG_TRACE("Initializing Particles for Disc {}...", this->toString());
    constexpr double pi = 3.14159265358979323846;

    // we interpreted radius 1 as already a ring around the centre
    // radius 0 --> particle in the centre
    std::array<double, 3> pos = x;
    std::array<double, 3> vel =
        ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2), std::plus<>());
    particles.addParticle(pos, vel, m);

    double circumference = 2 * pi * r;
    int numpoints = std::round(circumference / h);

    for (int i = 0; i < numpoints; i++) {
        std::array<double, 3> xyz;
        double angle = 2 * pi * i / numpoints;
        xyz[0] = x[0] + std::round(std::cos(angle) * r);
        xyz[1] = x[1] + std::round(std::sin(angle) * r);
        xyz[2] = 0;
        std::array<double, 3> velocity =
            ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2), std::plus<>());
        particles.addParticle(xyz, velocity, m);
    }

    for (double i = x[0] - r; i <= x[0] + r; i += h) {
        for (double j = x[1] - r; j <= x[1] + r; j += h) {
            if ((i - x[0]) * (i - x[0]) + (j - x[1]) * (j - x[1]) <= r * r) {
                std::array<double, 3> xyz;
                xyz[0] = i;
                xyz[1] = j;
                xyz[2] = 0;
                std::array<double, 3> velocity = ArrayUtils::elementWisePairOp(
                    v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2), std::plus<>());
                particles.addParticle(xyz, velocity, m);
            }
        }
    }
}

std::array<double, 3> &Disc::getX() { return x; }
const std::array<double, 3> &Disc::getX() const { return x; }
int Disc::getR() const { return r; }
double Disc::getH() const { return h; }
double Disc::getM() const { return m; }
std::array<double, 3> &Disc::getV() { return v; }
const std::array<double, 3> &Disc::getV() const { return v; }
double Disc::getMeanVelocity() const { return mean_velocity; }
ParticleContainer &Disc::getParticles() { return particles; }
const ParticleContainer &Disc::getParticles() const { return particles; }

bool Disc::operator==(const Disc &other) const {
    return (x == other.x) && (r == other.r) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles);
}
bool Disc::operator!=(const Disc &other) const { return !(*this == other); }

std::string Disc::toString() {
    std::stringstream stream;
    stream << "{ x: " << x << ", r: " << r << ", v: " << v << ", h: " << h << ", m: " << m
           << ", particles: " << particles.toString() << " }";
    return stream.str();
}