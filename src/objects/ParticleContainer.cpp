#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() {
    m_particles.reserve(16); // TODO REMOVE THIS!
};
ParticleContainer::ParticleContainer(size_t numParticles) { m_particles.reserve(numParticles); }

ContainerType::iterator ParticleContainer::begin() { return m_particles.begin(); }
ContainerType::iterator ParticleContainer::end() { return m_particles.end(); }
ContainerType::const_iterator ParticleContainer::begin() const { return m_particles.begin(); }
ContainerType::const_iterator ParticleContainer::end() const { return m_particles.end(); }

void ParticleContainer::addParticle(const Particle &particle) { m_particles.push_back(particle); }
void ParticleContainer::addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m) {
    m_particles.emplace_back(x, v, m);
}
size_t ParticleContainer::size() const { return m_particles.size(); }
ContainerType &ParticleContainer::getParticles() { return m_particles; }