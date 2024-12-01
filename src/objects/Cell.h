#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include <array>
#include <forward_list>
#include <string>

enum class CellType { INNER, BORDER, HALO };

class Cell {
  private:
    std::forward_list<Particle *> m_particles{};
    size_t m_particleCount{0};
    std::array<double, 3> m_size;
    std::array<double, 3> m_position;
    CellType m_type;
    int m_index;

  public:
    Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index);

    void addParticle(Particle *particle);
    void removeParticle(Particle *particle);
    void incrementParticleCount();
    void decrementParticleCount();

    const std::array<double, 3> &getX();
    size_t getParticleCount();
    std::forward_list<Particle *> &getParticles();
    CellType getType();
    int getIndex();
    std::string toString();
};