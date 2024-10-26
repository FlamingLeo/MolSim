/**
 * @file ParticleContainer.h
 * @brief Class for encapsulating and iterating over multiple Particle objects.
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/Particle.h"
#include <array>
#include <string>
#include <vector>

/// @brief The chosen data type for storing particles dynamically and contiguously is a std::vector.
using ContainerType = std::vector<Particle>;

class ParticleContainer {
  private:
    ContainerType m_particles;

  public:
    ContainerType::iterator begin();
    ContainerType::iterator end();
    ContainerType::const_iterator begin() const;
    ContainerType::const_iterator end() const;

    ParticleContainer();
    ParticleContainer(size_t num_particles);
    void addParticle(const Particle &particle);
    void addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m);
    void fromFile(const std::string &filename);
    size_t size() const;
    bool isEmpty() const;
    ContainerType &getParticles();
};