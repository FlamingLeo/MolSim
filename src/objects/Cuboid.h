//
// Created by marag on 11/5/2024.
//
#pragma once

#include "ParticleContainer.h"
#include <array>
#include <string>

class Cuboid {
  private:
    std::array<double, 3> position;
    std::array<int, 3> size;
    double h;
    double m;
    std::array<double, 3> v;
    double mean_velocity;
    ParticleContainer &particles;

  public:
    Cuboid(ParticleContainer &particles, const std::array<double, 3> &position, const std::array<int, 3> &size,
           const std::array<double, 3> &v, double h, double m);

    /* getters */
    std::array<double, 3> &getPosition();
    std::array<int, 3> &getSize();
    double getH();
    double getM();
    std::array<double, 3> &getV();
    double getMeanVelocity();
    ParticleContainer &getParticles();

    /* utility */
    bool operator==(const Cuboid &other) const;
    bool operator!=(const Cuboid &other) const;

    void initializeParticles();
    std::string toString();
};