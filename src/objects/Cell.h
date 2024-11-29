#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include <vector>

enum class CellType { INNER, BORDER, HALO };

class Cell {

  private:
    ParticleContainer particles;
    CellType type;
    std::array<double, 3> size;
    std::array<double, 3> position;

  public:
    Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type);

    void addParticle(Particle &newParticle);
    void removeParticle(Particle &removed);

    const std::array<double, 3> &getX();
    ParticleContainer &getParticles();
};