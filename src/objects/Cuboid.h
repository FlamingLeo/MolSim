//
// Created by marag on 11/5/2024.
//
#pragma once

#include "ParticleContainer.h"
#include <array>

class Cuboid {
  private:
    std::array<double, 3> position;
    std::array<int, 3> size;
    double h;
    double m;
    std::array<double, 3> v;
    double mean_velocity;
    ParticleContainer particles;

  public:
    Cuboid();

    Cuboid(const ParticleContainer &particles, const std::array<double, 3> &position, const std::array<int, 3> &size,
           const std::array<double, 3> &v, double h, double m);

    /* getters */
    std::array<double, 3> &getPosition();
    std::array<int, 3> &getSize();
    double getH();
    double getM();
    std::array<double, 3> &getV();
    double getMeanVelocity();
    ParticleContainer &getParticles();

    /* setters */
    void setPosition(const std::array<double, 3> &pos);
    void setSize(const std::array<int, 3> &s);
    void setH(double hh);
    void setM(double mm);
    void setV(const std::array<double, 3> &vel);
    void setMeanVelocity(double mv);
    void setParticles(const ParticleContainer &pc);

    /* utility */
    void initializeParticles();
    std::string toString();
};