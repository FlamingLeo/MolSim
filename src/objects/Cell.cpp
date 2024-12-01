#include "Cell.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <algorithm>
#include <sstream>
#include <string>

static std::string typeToString(CellType type) {
    switch (type) {
    case CellType::INNER:
        return "INNER";
    case CellType::BORDER:
        return "BORDER";
    case CellType::HALO:
        return "HALO";
    }
}

Cell::Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index)
    : m_type{type}, m_size{size}, m_position{position}, m_index{index} {};

void Cell::addParticle(Particle *particle) { m_particles.push_front(particle); }
void Cell::removeParticle(Particle *particle) { m_particles.remove(particle); }
void Cell::incrementParticleCount() { m_particleCount++; }
void Cell::decrementParticleCount() { m_particleCount--; }
const std::array<double, 3> &Cell::getX() { return m_position; }
size_t Cell::getParticleCount() { return m_particleCount; }
CellType Cell::getType() { return m_type; }
int Cell::getIndex() { return m_index; }
std::forward_list<Particle *> &Cell::getParticles() { return m_particles; }
std::string Cell::toString() {
    const std::array<double, 3> to{m_position[0] + m_size[0], m_position[1] + m_size[1], m_position[2] + m_size[2]};
    std::stringstream ss;
    ss << "[Type: " << typeToString(m_type) << ", Size: " << ArrayUtils::to_string(m_size)
       << ", Positions (from (incl.) -> to (excl.)): " << ArrayUtils::to_string(m_position) << " -> "
       << ArrayUtils::to_string(to) << "]";
    return ss.str();
}
