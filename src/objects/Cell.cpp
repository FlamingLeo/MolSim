#include "Cell.h"
#include "ParticleContainer.h"
#include <algorithm>

Cell::Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type) : type{type}, size{size}, position{position} {
    particles = ParticleContainer();
};


void Cell::addParticle(Particle &newParticle) {
    particles.addParticle(newParticle);
}

void Cell::removeParticle(Particle &removed) {
    particles.removeParticle(removed);
}

const std::array<double, 3> &Cell::getX()  { return position; }
ParticleContainer &Cell::getParticles() {return particles;}

