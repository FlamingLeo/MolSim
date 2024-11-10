//
// Created by marag on 11/5/2024.
//

#include "Cuboid.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include "utils/StringUtils.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Cuboid::Cuboid(ParticleContainer &particles, const std::array<double, 3> &position, const std::array<int, 3> &size,
               const std::array<double, 3> &v, double h, double m)
    : position{position}, size{size}, h{h}, m{m}, v{v}, mean_velocity{0.1}, particles{particles} {
    SPDLOG_TRACE("Generated Cuboid (simple constructor) - position: {}, size: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 StringUtils::fromArray(position), StringUtils::fromArray(size), h, m, StringUtils::fromArray(v),
                 mean_velocity);
}

void Cuboid::initializeParticles() {
    SPDLOG_TRACE("Initializing Particles for Cuboid {}...", this->toString());
    std::array<double, 3> xyz;
    for (int i = 0; i < size[2]; i++) {
        for (int j = 0; j < size[1]; j++) {
            for (int k = 0; k < size[0]; k++) {
                xyz = {position[0] + k * h, position[1] + j * h, position[2] + i * h};
                v = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 3),
                                                  std::plus<>());
                v[2] = 0;
                particles.addParticle(xyz, v, m);
            }
        }
    }
}

std::array<double, 3> &Cuboid::getPosition() { return position; }
std::array<int, 3> &Cuboid::getSize() { return size; }
double Cuboid::getH() { return h; }
double Cuboid::getM() { return m; }
std::array<double, 3> &Cuboid::getV() { return v; }
double Cuboid::getMeanVelocity() { return mean_velocity; }
ParticleContainer &Cuboid::getParticles() { return particles; }

void Cuboid::setPosition(const std::array<double, 3> &pos) { position = pos; }
void Cuboid::setSize(const std::array<int, 3> &s) { size = s; }
void Cuboid::setH(double hh) { h = hh; }
void Cuboid::setM(double mm) { m = mm; }
void Cuboid::setV(const std::array<double, 3> &vel) { v = vel; }
void Cuboid::setMeanVelocity(double mv) { mean_velocity = mv; }

bool Cuboid::operator==(const Cuboid &other) const {
    return (position == other.position) && (size == other.size) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles);
}
bool Cuboid::operator!=(const Cuboid &other) const { return !(*this == other); }

std::string Cuboid::toString() {
    std::stringstream stream;
    stream << "{ x: " << position << ", N: " << size << ", v: " << v << ", h: " << h << ", m: " << m
           << ", particles: " << particles.toString() << " }";
    return stream.str();
}
