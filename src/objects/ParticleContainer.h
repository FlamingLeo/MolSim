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
#include <vector>

class ParticleContainer {
  private:
    std::vector<Particle> m_particles;

  public:

    using ContainerType = std::vector<Particle>;

    ContainerType::iterator begin() { return m_particles.begin(); }
    ContainerType::iterator end()   { return m_particles.end(); }
    ContainerType::const_iterator begin() const { return m_particles.begin(); }
    ContainerType::const_iterator end() const { return m_particles.end(); }


    ParticleContainer();
    ParticleContainer(size_t num_particles);
    void addParticle(const Particle &particle);
    void addParticle(const std::array<double, 3>& x, const std::array<double, 3>& v, double m);
    size_t size() const;
    std::vector<Particle> &getParticles();
};