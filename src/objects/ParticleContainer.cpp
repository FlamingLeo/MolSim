#include "ParticleContainer.h"
#include "io/input/FileReader.h"

/* container constructors */
ParticleContainer::ParticleContainer() = default;
ParticleContainer::ParticleContainer(size_t numParticles) { m_particles.reserve(numParticles); }

/* iterator implementations */
ContainerType::iterator ParticleContainer::begin() { return m_particles.begin(); }
ContainerType::iterator ParticleContainer::end() { return m_particles.end(); }

ContainerType::const_iterator ParticleContainer::begin() const { return m_particles.begin(); }
ContainerType::const_iterator ParticleContainer::end() const { return m_particles.end(); }

/* pair iterator */
ParticleContainer::PairIterator::PairIterator(ContainerType::iterator outer, ContainerType::iterator inner,
                                              ContainerType::iterator begin, ContainerType::iterator end)
    : outer_it(outer), inner_it(inner), container_begin(begin), container_end(end) {}

std::pair<Particle &, Particle &> ParticleContainer::PairIterator::operator*() const { return {*outer_it, *inner_it}; }
ParticleContainer::PairIterator &ParticleContainer::PairIterator::operator++() {
    ++inner_it;
    if (inner_it == container_end) {
        ++outer_it;
        inner_it = container_begin;
    }
    return *this;
}
bool ParticleContainer::PairIterator::operator==(const ParticleContainer::PairIterator &other) const {
    return outer_it == other.outer_it && inner_it == other.inner_it;
}
bool ParticleContainer::PairIterator::operator!=(const ParticleContainer::PairIterator &other) const {
    return !(*this == other);
}

ParticleContainer::PairIterator ParticleContainer::beginPairs() {
    return PairIterator(m_particles.begin(), m_particles.begin(), m_particles.begin(), m_particles.end());
}
ParticleContainer::PairIterator ParticleContainer::endPairs() {
    return PairIterator(m_particles.end(), m_particles.begin(), m_particles.begin(), m_particles.end());
}

/* container functions */
void ParticleContainer::addParticle(const Particle &particle) { m_particles.push_back(particle); }
void ParticleContainer::addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m) {
    m_particles.emplace_back(x, v, m);
}
void ParticleContainer::fromFile(const std::string &filename) {
    FileReader fileReader(filename);
    fileReader.readFile(this);
};
void ParticleContainer::reserve(size_t capacity) { m_particles.reserve(capacity); }
size_t ParticleContainer::size() const { return m_particles.size(); }
bool ParticleContainer::isEmpty() const { return this->size() == 0; }
ContainerType &ParticleContainer::getParticles() { return m_particles; }