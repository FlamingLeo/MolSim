#include "ParticleContainer.h"
#include "io/input/FileReader.h"
#include "utils/ArrayUtils.h"
#include "utils/StringUtils.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <utility>

/* container constructors and destructor */
ParticleContainer::ParticleContainer() { SPDLOG_TRACE("Generated ParticleContainer (empty)."); }
ParticleContainer::ParticleContainer(size_t numParticles) {
    m_particles.reserve(numParticles);
    SPDLOG_TRACE("Generated ParticleContainer with {} spaces.", numParticles);
}
ParticleContainer::~ParticleContainer() { SPDLOG_TRACE("Destroyed ParticleContainer."); }

/* iterator implementations */
ParticleContainer::ContainerType::iterator ParticleContainer::begin() { return m_particles.begin(); }
ParticleContainer::ContainerType::iterator ParticleContainer::end() { return m_particles.end(); }

ParticleContainer::ContainerType::const_iterator ParticleContainer::begin() const { return m_particles.begin(); }
ParticleContainer::ContainerType::const_iterator ParticleContainer::end() const { return m_particles.end(); }

/* pair iterator */
ParticleContainer::PairIterator::PairIterator(ContainerType::iterator outer, ContainerType::iterator inner,
                                              ContainerType::iterator begin, ContainerType::iterator end)
    : outer_it(outer), inner_it(inner), container_begin(begin), container_end(end) {
    // handle 1-element container case
    if (container_end == container_begin + 1) {
        outer_it = container_end;
        inner_it = container_end;
    }
    if (outer_it != container_end)
        inner_it = outer_it + 1;
}
std::pair<Particle &, Particle &> ParticleContainer::PairIterator::operator*() const { return {*outer_it, *inner_it}; }
ParticleContainer::PairIterator &ParticleContainer::PairIterator::operator++() {
    ++inner_it;
    if (inner_it == container_end) {
        ++outer_it;
        if (outer_it == container_end - 1) {
            inner_it = container_end;
            outer_it = container_end;
        } else {
            inner_it = outer_it + 1;
        }
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
    return PairIterator(m_particles.end(), m_particles.end(), m_particles.begin(), m_particles.end());
}

/* container functions */
Particle &ParticleContainer::operator[](size_t index) { return m_particles[index]; }
const Particle &ParticleContainer::operator[](size_t index) const { return m_particles[index]; }

void ParticleContainer::addParticle(const Particle &particle) {
    m_particles.push_back(particle);
    SPDLOG_TRACE("Added Particle to ParticleContainer - {}", particle.toString());
}

void ParticleContainer::addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m) {
    m_particles.emplace_back(x, v, m);
    SPDLOG_TRACE("Created and added Particle to ParticleContainer - x: {}, v: {}, m: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), m);
}
void ParticleContainer::reserve(size_t capacity) {
    m_particles.reserve(capacity);
    SPDLOG_TRACE("Reserved {} spaces for ParticleContainer", capacity);
}
Particle &ParticleContainer::get(size_t index) {
    if (index < 0 || index >= m_particles.size())
        CLIUtils::error("Index out of bounds for ParticleContainer", StringUtils::fromNumber(index), false);
    return m_particles[index];
}
const Particle &ParticleContainer::get(size_t index) const {
    if (index < 0 || index >= m_particles.size())
        CLIUtils::error("Index out of bounds for ParticleContainer", StringUtils::fromNumber(index), false);
    return m_particles[index];
}
size_t ParticleContainer::size() const { return m_particles.size(); }
size_t ParticleContainer::activeSize() const {
    return std::count_if(m_particles.begin(), m_particles.end(), [](const Particle &p) { return p.isActive(); });
}
bool ParticleContainer::isEmpty() const { return this->size() == 0; }
ParticleContainer::ContainerType &ParticleContainer::getParticles() { return m_particles; }
bool ParticleContainer::operator==(const ParticleContainer &other) const { return m_particles == other.m_particles; }
bool ParticleContainer::operator!=(const ParticleContainer &other) const { return m_particles != other.m_particles; }
std::string ParticleContainer::toString() const {
    std::stringstream ss;
    ss << "{ ";
    for (size_t i = 0; i < m_particles.size(); ++i) {
        ss << m_particles[i].toString();
        if (i != m_particles.size() - 1)
            ss << ", ";
    }
    ss << " }";
    return ss.str();
}