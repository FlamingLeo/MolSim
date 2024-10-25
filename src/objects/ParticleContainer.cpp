#include "ParticleContainer.h"

// iterator functions
ParticleContainer::Iterator::Iterator(Particle *ptr) : m_ptr(ptr) {}
Particle &ParticleContainer::Iterator::operator*() const { return *m_ptr; }
Particle *ParticleContainer::Iterator::operator->() { return m_ptr; }
ParticleContainer::Iterator &ParticleContainer::Iterator::operator++() {
    m_ptr++;
    return *this;
}
ParticleContainer::Iterator ParticleContainer::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}
bool operator==(const ParticleContainer::Iterator &a, const ParticleContainer::Iterator &b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const ParticleContainer::Iterator &a, const ParticleContainer::Iterator &b) {
    return a.m_ptr != b.m_ptr;
}

// particle container functions
ParticleContainer::ParticleContainer() {
    m_particles.reserve(16); // TODO REMOVE THIS!
};
ParticleContainer::ParticleContainer(size_t numParticles) {
    m_particles.reserve(numParticles);
}

void ParticleContainer::addParticle(const Particle &particle) { m_particles.push_back(particle); }
void ParticleContainer::addParticle(const std::array<double, 3>& x, const std::array<double, 3>& v, double m) { m_particles.emplace_back(x,v,m); }
size_t ParticleContainer::size() { return m_particles.size(); }
std::vector<Particle> &ParticleContainer::getParticles() { return m_particles; }