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

    class Iterator {
      private:
        Particle *m_ptr;

      public:
        Iterator(Particle *ptr);
        Particle &operator*() const;
        Particle *operator->();
        Iterator &operator++();
        Iterator operator++(int);
        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        }
        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        }
    };

  public:
    void addParticle(const Particle &particle);
    size_t size();
    std::vector<Particle> &getParticles();
    Iterator begin() { return Iterator(&m_particles[0]); }
    Iterator end() { return Iterator(&m_particles[0] + m_particles.size()); }
};