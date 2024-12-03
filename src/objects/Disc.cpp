#include "Disc.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "utils/StringUtils.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Disc::Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r, const std::array<double, 3> &v,
           double h, double m)
    : x{x}, r{r}, h{h}, m{m}, v{v}, mean_velocity{0.1}, particles{particles} {
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
                SPDLOG_TRACE("Position {} is within circle, adding...", ArrayUtils::to_string({x, y, 0.0}));
                points.push_back({x, y, 0});
            } else {
                SPDLOG_TRACE("Position {} is NOT within circle, skipping...", ArrayUtils::to_string({x, y, 0.0}));
            }
        }
    }
    return points;
}

/* documented functions start here  */
void Disc::initializeDisc() {
    SPDLOG_TRACE("Initializing Particles for Disc {}...", this->toString());
    std::vector<std::array<double, 3>> positions = getCircleCoordinates(x[0], x[1], r, h);
    for (auto &p : positions) {
        std::array<double, 3> vel =
            ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2), std::plus<>());
        particles.addParticle(p, vel, m);
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