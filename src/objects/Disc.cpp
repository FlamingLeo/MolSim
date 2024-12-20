#include "Disc.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "utils/StringUtils.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

Disc::Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r, const std::array<double, 3> &v,
           double h, double m, int type, double epsilon, double sigma)
    : Cluster(particles, x, v, h, m, type, epsilon, sigma), r{r} {
    if (r < 0)
        CLIUtils::error("Radius must be 0 or greater, got", StringUtils::fromNumber(r));
    if (h <= 0)
        CLIUtils::error("Spacing must be greater than 0, got", StringUtils::fromNumber(h));
    SPDLOG_TRACE("Generated Disc (simple constructor) - x: {}, r: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 ArrayUtils::to_string(x), r, h, m, ArrayUtils::to_string(v), mean_velocity);
}

std::vector<std::array<double, 3>> Disc::getCircleCoordinates(double centerX, double centerY, double radius,
                                                              double distance) {
    std::vector<std::array<double, 3>> points;
    double minX = centerX - radius;
    double maxX = centerX + radius;
    double minY = centerY - radius;
    double maxY = centerY + radius;
    for (double y = minY; y <= maxY; y += distance) {
        for (double x = minX; x <= maxX; x += distance) {
            if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
                SPDLOG_TRACE("Position ({}, {}) is within circle, adding...", x, y);
                points.push_back({x, y, 0});
            } else {
                SPDLOG_TRACE("Position ({}, {}) is NOT within circle, skipping...", x, y);
            }
        }
    }
    return points;
}

/* documented functions start here  */
void Disc::initialize(size_t dimensions) {
    SPDLOG_TRACE("Initializing Particles for Disc {}...", this->toString());
    std::vector<std::array<double, 3>> positions = getCircleCoordinates(x[0], x[1], r, h);
    for (auto &p : positions) {
        std::array<double, 3> vel = ArrayUtils::elementWisePairOp(
            v, maxwellBoltzmannDistributedVelocity(mean_velocity, dimensions), std::plus<>());
        particles.addParticle(p, vel, m, type, epsilon, sigma);
    }
}

int Disc::getR() const { return r; }
bool Disc::operator==(const Disc &other) const {
    return (x == other.x) && (r == other.r) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles) && (sigma == other.sigma) &&
           (epsilon == other.epsilon);
}
bool Disc::operator!=(const Disc &other) const { return !(*this == other); }
std::string Disc::toString() {
    std::stringstream stream;
    stream << "{ x: " << x << ", r: " << r << ", v: " << v << ", h: " << h << ", m: " << m << ", sigma: " << sigma
           << ", epsilon: " << epsilon << ", particles: " << particles.toString() << " }";
    return stream.str();
}