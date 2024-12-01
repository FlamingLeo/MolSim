#include "Cell.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

Cell::Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index,
           const std::vector<HaloLocation> &haloLocation)
    : m_type{type}, m_size{size}, m_position{position}, m_index{index}, m_haloLocation{haloLocation} {};

void Cell::addParticle(Particle *particle) { m_particles.push_front(particle); }
void Cell::removeParticle(Particle *particle) { m_particles.remove(particle); }
const std::array<double, 3> &Cell::getSize() const { return m_size; }
const std::array<double, 3> &Cell::getX() const { return m_position; }
HaloLocation Cell::getCornerRegion(const Particle &p) {
    // verify that this is a corner cell
    assert(!(this->m_haloLocation.empty()));

    // get the relative x and y positions of the particle inside the cell
    double relX = p.getX()[0] - m_position[0];
    double relY = p.getX()[1] - m_position[1];
    assert(relX < 0 || relX > m_size[0] || relY < 0 || relY > m_size[1]);

    // check what type of corner cell this is
    bool NW = VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    bool SE = VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool NE = VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool SW = VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    bool NW_SE = NW || SE; // diagonal top left -> bottom right
    bool NE_SW = NE || SW; // diagonal bottom left -> top right
    assert(NW_SE || NE_SW);

    // handle different diagonal cases
    bool aboveDiagonal = NE_SW ? (relX <= relY) : (relX > relY);
    if (NE) {
        return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::EAST;
    } else if (SW) {
        return aboveDiagonal ? HaloLocation::WEST : HaloLocation::SOUTH;
    } else if (NW) {
        return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::WEST;
    } else if (SE) {
        return aboveDiagonal ? HaloLocation::EAST : HaloLocation::SOUTH;
    } else {
        CLIUtils::error("Invalid corner cell! This should NOT happen.", "", false);
        return HaloLocation::NORTH; // for certain compilers
    }
}
CellType Cell::getType() { return m_type; }
int Cell::getIndex() { return m_index; }
std::vector<HaloLocation> &Cell::getHaloLocation() { return m_haloLocation; }
std::forward_list<Particle *> &Cell::getParticles() { return m_particles; }
std::string Cell::toString() {
    const std::array<double, 3> to{m_position[0] + m_size[0], m_position[1] + m_size[1], m_position[2] + m_size[2]};
    std::stringstream ss;
    ss << "[Type: " << CellUtils::fromType(m_type) << ", Size: " << ArrayUtils::to_string(m_size)
       << ", Positions (from (incl.) -> to (excl.)): " << ArrayUtils::to_string(m_position) << " -> "
       << ArrayUtils::to_string(to)
       << (!m_haloLocation.empty() ? (", Halo: " + CellUtils::fromHaloVec(m_haloLocation)) : "") << "]";
    return ss.str();
}
