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
           const std::vector<HaloLocation> &haloLocation, const std::vector<BorderLocation> &borderLocation)
    : m_type{type}, m_size{size}, m_position{position}, m_index{index}, m_haloLocation{haloLocation},
      m_borderLocation{borderLocation} {};
std::forward_list<std::reference_wrapper<Particle>>::iterator Cell::begin() { return m_particles.begin(); }
std::forward_list<std::reference_wrapper<Particle>>::iterator Cell::end() { return m_particles.end(); }
void Cell::addParticle(Particle &particle) { m_particles.push_front(std::ref(particle)); }
void Cell::removeParticle(Particle &particle) {
    m_particles.remove_if([&particle](const std::reference_wrapper<Particle> &p) { return &p.get() == &particle; });
}
const std::array<double, 3> &Cell::getSize() const { return m_size; }
const std::array<double, 3> &Cell::getX() const { return m_position; }
HaloLocation Cell::getCornerRegion(const Particle &p) const {

    //return m_haloLocation[0];

    // verify that this is a corner cell
    assert(!(this->m_haloLocation.empty()));

    if (m_size[2] == 0) {
        // get the relative x and y positions of the particle inside the cell
        double relX = p.getX()[0] - m_position[0];
        double relY = p.getX()[1] - m_position[1];
        SPDLOG_DEBUG("posX: {}, posY: {}, relX: {}, relY: {}, sizeX: {}, sizeY: {}", m_position[0], m_position[1], relX,
                     relY, m_size[0], m_size[1]);
        assert(!(relX < 0 || relX > m_size[0] || relY < 0 || relY > m_size[1]));

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
        }
        if (SW) {
            return aboveDiagonal ? HaloLocation::WEST : HaloLocation::SOUTH;
        }
        if (NW) {
            return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::WEST;
        }
        if (SE) {
            return aboveDiagonal ? HaloLocation::EAST : HaloLocation::SOUTH;
        }
        CLIUtils::error("Invalid corner cell! This should NOT happen.", "", false);
        return HaloLocation::NORTH; // for certain compilers

    } else {
        //HERE THE SAME BUT FOR 3D
        double relX = p.getX()[0] - m_position[0];
        double relY = p.getX()[1] - m_position[1];
        double relZ = p.getX()[2] - m_position[2];
        SPDLOG_DEBUG("posX: {}, posY: {}, posZ: {}, relX: {}, relY: {}, relZ: {}, sizeX: {}, sizeY: {}, sizeZ: {}", m_position[0], m_position[1], m_position[2], relX,
                     relY, relZ, m_size[0], m_size[1], m_size[2]);
        assert(!(relX < 0 || relX > m_size[0] || relY < 0 || relY > m_size[1] || relZ < 0 || relZ > m_size[2]));

        if(m_haloLocation.size() == 2){
            //we determine the character of the diagonal
            bool NW = VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool SE = VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool NE = VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool SW = VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool BW = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool BE = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool BS = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH);
            bool BN = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH);
            bool AW = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool AE = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool AS = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH);
            bool AN = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH);

            bool NW_SE = NW || SE; // diagonal top left -> bottom right
            bool NE_SW = NE || SW; // diagonal bottom left -> top right

            bool BW_AE = BW || AE;
            bool BE_AW = BE || AW;

            bool BS_AN = BS || AN;
            bool BN_AS = BN || AS;

            //bool aboveDiagonal = NE_SW ? (relX <= relY) : (relX > relY);
            bool aboveDiagonal = false;
            if (NE_SW){
                aboveDiagonal = relX <= relY;
                //NE
                if (NE){
                    return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::EAST;
                }
                //SW
                return aboveDiagonal ? HaloLocation::WEST : HaloLocation::SOUTH;
            } else if (NW_SE){
                aboveDiagonal = relX > relY;
                //NW
                if (NW){
                    return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::WEST;
                }
                //SE
                return aboveDiagonal ? HaloLocation::EAST : HaloLocation::SOUTH;
            } else if (BW_AE){
                aboveDiagonal = relX <= relZ;
                //BW
                if (BW){
                    return aboveDiagonal? HaloLocation::WEST : HaloLocation::BELOW;
                }
                //AE
                return aboveDiagonal ? HaloLocation::ABOVE : HaloLocation::EAST;
            } else if (BE_AW){
                aboveDiagonal = relX > relZ;
                //BE
                if (BE){
                    return aboveDiagonal ? HaloLocation::EAST : HaloLocation::BELOW;
                }
                //AW
                return aboveDiagonal ? HaloLocation::ABOVE : HaloLocation::WEST;
            } else if (BS_AN){
                aboveDiagonal = relZ <= relY;
                //BS
                if (BS){
                    return aboveDiagonal ? HaloLocation::BELOW : HaloLocation::SOUTH;
                }
                //AN
                return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::ABOVE;
            } else if (BN_AS){
                aboveDiagonal = relZ > relY;
                //BN
                if (BN){
                    return aboveDiagonal ? HaloLocation::NORTH : HaloLocation::BELOW;
                }
                //AS
                return aboveDiagonal ? HaloLocation::ABOVE : HaloLocation::SOUTH;
            }



        } else {
            //The 8 3-fold corners
            bool BSE = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool BSW = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool BNE = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool BNW = VEC_CONTAINS(m_haloLocation, HaloLocation::BELOW) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool ASE = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool ASW = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::SOUTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);
            bool ANE = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::EAST);
            bool ANW = VEC_CONTAINS(m_haloLocation, HaloLocation::ABOVE) && VEC_CONTAINS(m_haloLocation, HaloLocation::NORTH) && VEC_CONTAINS(m_haloLocation, HaloLocation::WEST);

            if (BSE){
                bool BS = 0.5 * relX  - 0.5 * relZ + relY >= 0.5;
                bool BE = relX > relZ;

                if (BS){
                    return BE ? HaloLocation::EAST : HaloLocation::BELOW;
                }
                return HaloLocation::SOUTH;
            }
            if (BSW){
                bool BS = -0.5 * relX - 0.5 * relZ + relY >= 0;
                bool BW = relX <= relZ;

                if (BS){
                    return BW ? HaloLocation::WEST : HaloLocation::BELOW;
                }
                return HaloLocation::SOUTH;
            }
            if (BNE){
                bool BN = -0.5 * relX + 0.5 * relZ + relY >= 0.5;
                bool BE = relX > relZ;

                if (BN){
                    return HaloLocation::NORTH;
                }
                return BE ? HaloLocation::EAST : HaloLocation::BELOW;
            }
            if (BNW){
                bool BN = 0.5 * relX + 0.5 * relZ + relY >= 1;
                bool BW = relX <= relZ;

                if (BN){
                    return HaloLocation::NORTH;
                }
                return BW ? HaloLocation::WEST : HaloLocation::BELOW;
            }
            if (ASE){
                bool AS = 0.5 * relX + 0.5 * relZ + relY >= 1;
                bool AE = relX <= relZ;

                if (AS){
                    return AE ? HaloLocation::ABOVE : HaloLocation::EAST;
                }
                return HaloLocation::SOUTH;
            }
            if (ASW){
                bool AS = -0.5 * relX + 0.5 * relZ + relY >= 0.5;
                bool AW = relX > relZ;

                if (AS){
                    return AW ? HaloLocation::ABOVE : HaloLocation::WEST;
                }
                return HaloLocation::SOUTH;
            }
            if (ANE){
                bool AN = -0.5 * relX - 0.5 * relZ + relY >= 0;
                bool AE = relX <= relZ;

                if (AN){
                    return HaloLocation::NORTH;
                }
                return AE ? HaloLocation::ABOVE : HaloLocation::EAST;
            }
            if (ANW){
                bool AN = 0.5 * relX - 0.5 * relZ + relY >= 0.5;
                bool AW = relX > relZ;

                if (AN){
                    return HaloLocation::NORTH;
                }
                return AW ? HaloLocation::ABOVE : HaloLocation::WEST;
            }
        }
    }
    return m_haloLocation[0];
}

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
