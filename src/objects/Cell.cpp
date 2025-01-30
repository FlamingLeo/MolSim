#include "Cell.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#define X size_t[0]
#define Y size_t[1]
#define Z size_t[2]

#define C1() ((X*X + Y*Y) / (X*X + Y*Y + Z*Z))

//these are the equations for the triple corner vertical planes, just for a bit of lucidity
//the N's are the plane normal vectors and RHS the right hand side to the plane equation
//so the condition would be the dot product of the normal and the actual coordinates >= RHS
//e.g.(no dot product available): (1 - C1) * X * relX + C1 * Y * relY + (C1 - 1) * Z * relZ >= RHS_BSE_ANW
#define N_BSE_ANW {(1 - C1) * X, C1 * Y, (C1 - 1) * Z}
#define RHS_BSE_ANW ((1 - C1) * X * X)

#define N_BSW_ANE {(C1 - 1) * X, C1 * Y, (C1 - 1) * Z}
#define RHS_BSW_ANE 0.0

#define N_BNE_ASW {(C1 - 1) * X, C1 * Y, (1 - C1) * Z}
#define RHS_BNE_ASW ((1 - C1) * Z * Z)

#define N_BNW_ASE {(1 - C1) * X, C1 * Y, (1 - C1) * Z}
#define RHS_BNW_ASE (C1 * Y * Y)

#define RETURN_DIAG_CONDITION(cond1, cond2, adCond, loc1, loc2, loc3, loc4)                                            \
    if ((cond1) || (cond2)) {                                                                                          \
        bool aboveDiagonal = (adCond);                                                                                 \
        if ((cond1)) {                                                                                                 \
            return aboveDiagonal ? (loc1) : (loc2);                                                                    \
        }                                                                                                              \
        return aboveDiagonal ? (loc3) : (loc4);                                                                        \
    }
#define RETURN_TRIPLE_DIAG_CONDITION(cond0, cond1, cond2, loc1, loc2, loc3)                                            \
    if ((cond0)) {                                                                                                     \
        bool b1 = (cond1);                                                                                             \
        bool b2 = (cond2);                                                                                             \
        if (b1)                                                                                                        \
            return b2 ? (loc1) : (loc2);                                                                               \
        return (loc3);                                                                                                 \
    }
#define RETURN_TRIPLE_DIAG_REVERSE(cond0, cond1, cond2, loc1, loc2, loc3)                                              \
    if ((cond0)) {                                                                                                     \
        bool b1 = (cond1);                                                                                             \
        bool b2 = (cond2);                                                                                             \
        if (b1)                                                                                                        \
            return (loc1);                                                                                             \
        return b2 ? (loc2) : (loc3);                                                                                   \
    }

/* internals */
Cell::Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index,
           const std::vector<HaloLocation> &haloLocation, const std::vector<BorderLocation> &borderLocation)
    : m_type{type}, m_size{size}, m_position{position}, m_index{index}, m_haloLocation{haloLocation},
      m_borderLocation{borderLocation} {};
std::forward_list<std::reference_wrapper<Particle>>::iterator Cell::begin() { return m_particles.begin(); }
std::forward_list<std::reference_wrapper<Particle>>::iterator Cell::end() { return m_particles.end(); }

/* corner region helper functions */
std::array<double, 3> Cell::getRelativePosition(const Particle &p) const {
    return {p.getX()[0] - m_position[0], p.getX()[1] - m_position[1],
            m_size[2] > 0 ? p.getX()[2] - m_position[2] : 0.0};
}
void Cell::validatePosition(const std::array<double, 3> &relPos) const {
    assert(relPos[0] >= 0 && relPos[0] <= m_size[0]);
    assert(relPos[1] >= 0 && relPos[1] <= m_size[1]);
    if (m_size[2] > 0) {
        assert(relPos[2] >= 0 && relPos[2] <= m_size[2]);
    }
}
bool Cell::isCorner(HaloLocation loc1, HaloLocation loc2) const {
    return VEC_CONTAINS(m_haloLocation, loc1) && VEC_CONTAINS(m_haloLocation, loc2);
}
HaloLocation Cell::handle2DCorner(const std::array<double, 3> &relPos) const {
    bool NW = isCorner(HaloLocation::NORTH, HaloLocation::WEST);
    bool SE = isCorner(HaloLocation::SOUTH, HaloLocation::EAST);
    bool NE = isCorner(HaloLocation::NORTH, HaloLocation::EAST);
    bool SW = isCorner(HaloLocation::SOUTH, HaloLocation::WEST);
    bool NW_SE = NW || SE;
    bool NE_SW = NE || SW;
    assert(NW_SE || NE_SW);

    bool aboveDiagonal = NE_SW ? (relPos[0] <= relPos[1]) : (relPos[0] > relPos[1]);
    if (NE)
        return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::EAST;
    if (SW)
        return aboveDiagonal ? HaloLocation::WEST : HaloLocation::SOUTH;
    if (NW)
        return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::WEST;
    if (SE)
        return aboveDiagonal ? HaloLocation::EAST : HaloLocation::SOUTH;

    CLIUtils::error("Invalid corner cell! This should NOT happen.", "", false);
    return HaloLocation::NORTH; // fallback
}
HaloLocation Cell::handle3DCorner(const std::array<double, 3> &relPos) const {
    return m_haloLocation.size() == 2 ? handle3DDiagonal(relPos) : handle3DTripleCorner(relPos);
}
HaloLocation Cell::handle3DDiagonal(const std::array<double, 3> &relPos) const {
    bool NW = isCorner(HaloLocation::NORTH, HaloLocation::WEST);
    bool SE = isCorner(HaloLocation::SOUTH, HaloLocation::EAST);
    bool NE = isCorner(HaloLocation::NORTH, HaloLocation::EAST);
    bool SW = isCorner(HaloLocation::SOUTH, HaloLocation::WEST);
    bool BW = isCorner(HaloLocation::BELOW, HaloLocation::WEST);
    bool BE = isCorner(HaloLocation::BELOW, HaloLocation::EAST);
    bool BS = isCorner(HaloLocation::BELOW, HaloLocation::SOUTH);
    bool BN = isCorner(HaloLocation::BELOW, HaloLocation::NORTH);
    bool AW = isCorner(HaloLocation::ABOVE, HaloLocation::WEST);
    bool AE = isCorner(HaloLocation::ABOVE, HaloLocation::EAST);
    bool AS = isCorner(HaloLocation::ABOVE, HaloLocation::SOUTH);
    bool AN = isCorner(HaloLocation::ABOVE, HaloLocation::NORTH);
    RETURN_DIAG_CONDITION(NE, SW, (relPos[0] <= relPos[1]), HaloLocation::NORTH, HaloLocation::EAST, HaloLocation::WEST,
                          HaloLocation::SOUTH);
    RETURN_DIAG_CONDITION(NW, SE, (relPos[0] > relPos[1]), HaloLocation::NORTH, HaloLocation::WEST, HaloLocation::EAST,
                          HaloLocation::SOUTH);
    RETURN_DIAG_CONDITION(BW, AE, (relPos[0] <= relPos[2]), HaloLocation::WEST, HaloLocation::BELOW,
                          HaloLocation::ABOVE, HaloLocation::EAST);
    RETURN_DIAG_CONDITION(BE, AW, (relPos[0] > relPos[2]), HaloLocation::EAST, HaloLocation::BELOW, HaloLocation::ABOVE,
                          HaloLocation::WEST);
    RETURN_DIAG_CONDITION(BS, AN, (relPos[2] <= relPos[1]), HaloLocation::BELOW, HaloLocation::SOUTH,
                          HaloLocation::NORTH, HaloLocation::ABOVE);
    RETURN_DIAG_CONDITION(BN, AS, (relPos[2] > relPos[1]), HaloLocation::NORTH, HaloLocation::BELOW,
                          HaloLocation::ABOVE, HaloLocation::SOUTH);
    CLIUtils::error("Invalid diagonal corner! This should NOT happen.", "", false);
    return HaloLocation::NORTH; // fallback
}
HaloLocation Cell::handle3DTripleCorner(const std::array<double, 3> &relPos) const {
    bool BSE = isCorner(HaloLocation::BELOW, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool BSW = isCorner(HaloLocation::BELOW, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    bool BNE = isCorner(HaloLocation::BELOW, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool BNW = isCorner(HaloLocation::BELOW, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    bool ASE = isCorner(HaloLocation::ABOVE, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool ASW = isCorner(HaloLocation::ABOVE, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    bool ANE = isCorner(HaloLocation::ABOVE, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
    bool ANW = isCorner(HaloLocation::ABOVE, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
    double relX = relPos[0], relY = relPos[1], relZ = relPos[2];

    RETURN_TRIPLE_DIAG_CONDITION(BSE, (0.5 * relX - 0.5 * relZ + relY >= 0.5), (relX > relZ), HaloLocation::EAST,
                                 HaloLocation::BELOW, HaloLocation::SOUTH);
    RETURN_TRIPLE_DIAG_CONDITION(BSW, (-0.5 * relX - 0.5 * relZ + relY >= 0), (relX <= relZ), HaloLocation::WEST,
                                 HaloLocation::BELOW, HaloLocation::SOUTH);
    RETURN_TRIPLE_DIAG_REVERSE(BNE, (-0.5 * relX + 0.5 * relZ + relY >= 0.5), (relX > relZ), HaloLocation::NORTH,
                               HaloLocation::EAST, HaloLocation::BELOW);
    RETURN_TRIPLE_DIAG_REVERSE(BNW, (0.5 * relX + 0.5 * relZ + relY >= 1), (relX <= relZ), HaloLocation::NORTH,
                               HaloLocation::WEST, HaloLocation::BELOW);
    RETURN_TRIPLE_DIAG_CONDITION(ASE, (0.5 * relX + 0.5 * relZ + relY >= 1), (relX <= relZ), HaloLocation::ABOVE,
                                 HaloLocation::EAST, HaloLocation::SOUTH);
    RETURN_TRIPLE_DIAG_CONDITION(ASW, (-0.5 * relX + 0.5 * relZ + relY >= 0.5), (relX > relZ), HaloLocation::ABOVE,
                                 HaloLocation::WEST, HaloLocation::SOUTH);
    RETURN_TRIPLE_DIAG_REVERSE(ANE, (-0.5 * relX - 0.5 * relZ + relY >= 0), (relX <= relZ), HaloLocation::NORTH,
                               HaloLocation::ABOVE, HaloLocation::EAST);
    RETURN_TRIPLE_DIAG_REVERSE(ANW, (0.5 * relX - 0.5 * relZ + relY >= 0.5), (relX > relZ), HaloLocation::NORTH,
                               HaloLocation::ABOVE, HaloLocation::WEST);
    CLIUtils::error("Invalid triple corner! This should NOT happen.", "", false);
    return HaloLocation::NORTH; // fallback
}
/* main corner region dispatch function */
HaloLocation Cell::getCornerRegion(const Particle &p) const {
    // verify that this is a corner cell
    assert(!(this->m_haloLocation.empty()));
    auto relPos = getRelativePosition(p);
    SPDLOG_DEBUG("posX: {}, posY: {}, posZ: {}, relX: {}, relY: {}, relZ: {}, sizeX: {}, sizeY: {}, sizeZ: {}",
                 m_position[0], m_position[1], m_position[2], relPos[0], relPos[1], relPos[2], m_size[0], m_size[1],
                 m_size[2]);
    validatePosition(relPos);

    // handle 2D or 3D simulation respectively
    return m_size[2] == 0 ? handle2DCorner(relPos) : handle3DCorner(relPos);
}

/* functionality */
void Cell::addParticle(Particle &particle) { m_particles.push_front(std::ref(particle)); }
void Cell::removeParticle(Particle &particle) {
    m_particles.remove_if([&particle](const std::reference_wrapper<Particle> &p) { return &p.get() == &particle; });
}
const std::array<double, 3> &Cell::getSize() const { return m_size; }
const std::array<double, 3> &Cell::getX() const { return m_position; }
CellType Cell::getType() const { return m_type; }
int Cell::getIndex() const { return m_index; }
const std::vector<HaloLocation> &Cell::getHaloLocation() const { return m_haloLocation; }
const std::vector<BorderLocation> &Cell::getBorderLocation() const { return m_borderLocation; }
std::vector<int> &Cell::getNeighbors() { return m_neighbors; }
const std::vector<int> &Cell::getNeighbors() const { return m_neighbors; }
std::forward_list<std::reference_wrapper<Particle>> &Cell::getParticles() { return m_particles; }
const std::forward_list<std::reference_wrapper<Particle>> &Cell::getParticles() const { return m_particles; }
std::string Cell::toString() const {
    const std::array<double, 3> to{m_position[0] + m_size[0], m_position[1] + m_size[1], m_position[2] + m_size[2]};
    std::stringstream ss;
    ss << "[Index: " << m_index << ", Type: " << CellUtils::fromType(m_type)
       << ", Size: " << ArrayUtils::to_string(m_size)
       << ", Positions (from (incl.) -> to (excl.)): " << ArrayUtils::to_string(m_position) << " -> "
       << ArrayUtils::to_string(to)
       << (!m_haloLocation.empty() ? (", Halo: " + CellUtils::fromHaloVec(m_haloLocation)) : "") << "]";
    return ss.str();
}