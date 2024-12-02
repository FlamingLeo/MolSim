#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include "utils/CellUtils.h"
#include <algorithm>
#include <array>
#include <bitset>
#include <forward_list>
#include <sstream>
#include <string>
#include <vector>
#define VEC_CONTAINS(vec, el) ((std::find((vec).begin(), (vec).end(), (el)) != (vec).end()))

class Cell {
    using ContainerType = std::forward_list<Particle *>;

  private:
    ContainerType m_particles{};
    std::array<double, 3> m_size;
    std::array<double, 3> m_position;
    std::vector<HaloLocation> m_haloLocation;
    CellType m_type;
    int m_index;

  public:
    Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index,
         const std::vector<HaloLocation> &haloLocation);

    ContainerType::iterator begin();
    ContainerType::iterator end();

    void addParticle(Particle *particle);
    void removeParticle(Particle *particle);

    /**
     * @brief
     *
     * See the report and presentation slides for more information.
     *
     * @param p
     * @return HaloLocation
     */
    HaloLocation getCornerRegion(const Particle &p);
    const std::array<double, 3> &getX() const;
    const std::array<double, 3> &getSize() const;
    std::vector<HaloLocation> &getHaloLocation();
    std::forward_list<Particle *> &getParticles();
    CellType getType();
    int getIndex();
    std::string toString();
};