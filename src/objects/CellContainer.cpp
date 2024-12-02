#include "CellContainer.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

/* constructor */
CellContainer::CellContainer(const std::array<double, 3> &domainSize,
                             const std::array<BoundaryCondition, 6> &conditions, double cutoff, ParticleContainer &pc,
                             size_t dim)
    : domainSize{domainSize}, cellSize{0, 0, 0}, numCells{1, 1, 1}, conditions{conditions}, cutoff{cutoff},
      particles{pc} {
    // check correct dimensions (could probably be a boolean instead...)
    if (dim < 2 || dim > 3)
        CLIUtils::error("Invalid cell container dimensions! (must be 2 or 3)", StringUtils::fromNumber(dim));

    // check that domain size and cutoff are initialized
    if (cutoff == INF)
        CLIUtils::error("Cutoff radius not initialized!");
    if (domainSize[0] == INF || domainSize[1] == INF || domainSize[2] == INF)
        CLIUtils::error("Domain size not initialized!");

    SPDLOG_TRACE("Generating CellContainer with domain size {} and cutoff radius {} (in {} dimensions)",
                 ArrayUtils::to_string(domainSize), cutoff, dim);

    // determining cell size
    for (size_t i = 0; i < dim; i++) {
        if (std::fabs(std::fmod(domainSize[i], cutoff)) < 1e-9) {
            // perfect fit
            cellSize[i] = cutoff;
            numCells[i] = std::round(domainSize[i] / cellSize[i]) + 2;
            SPDLOG_DEBUG("Cutoff perfectly divides domain size, using cutoff radius {} as cell size for dimension {}.",
                         cutoff, i);
        } else {
            // we round up cellsize so that cell > cutoff and we only search adjacent cells
            cellSize[i] = domainSize[i] / std::floor(domainSize[i] / cutoff);
            numCells[i] = std::round(domainSize[i] / cellSize[i]) + 2;
            SPDLOG_DEBUG("Cutoff does NOT perfectly divide domain size, using {} as cell size for dimension {}.",
                         cellSize[i], i);
        }
    }

    // reserve space for all cells
    size_t totalNumCells = numCells[0] * numCells[1] * numCells[2];
    cells.reserve(totalNumCells);
    SPDLOG_DEBUG("Reserved space for {} cells (X: {}, Y: {}, Z: {}).", totalNumCells, numCells[0], numCells[1],
                 numCells[2]);

    // creating cells
    size_t index = 0;
    for (int z = 0; z < numCells[2]; z++) {
        for (int y = 0; y < numCells[1]; y++) {
            for (int x = 0; x < numCells[0]; x++) {
                // set type of cell
                bool aboveHalo = z == (numCells[2] - 1);
                bool belowHalo = z == 0;
                bool northHalo = y == (numCells[1] - 1);
                bool southHalo = y == 0;
                bool westHalo = x == 0;
                bool eastHalo = x == (numCells[0] - 1);

                std::vector<HaloLocation> haloLocation;
                if (northHalo)
                    haloLocation.push_back(HaloLocation::NORTH);
                if (southHalo)
                    haloLocation.push_back(HaloLocation::SOUTH);
                if (westHalo)
                    haloLocation.push_back(HaloLocation::WEST);
                if (eastHalo)
                    haloLocation.push_back(HaloLocation::EAST);
                if (dim == 3) {
                    if (aboveHalo)
                        haloLocation.push_back(HaloLocation::ABOVE);
                    if (belowHalo)
                        haloLocation.push_back(HaloLocation::BELOW);
                }

                bool border;
                if (dim == 3) {
                    border = z == 1 || z == (numCells[2] - 2) || y == 1 || y == (numCells[1] - 2) || x == 1 ||
                             x == (numCells[0] - 2);
                } else {
                    border = y == 1 || y == (numCells[1] - 2) || x == 1 || x == (numCells[0] - 2);
                };
                CellType type;
                if (!haloLocation.empty()) {
                    type = CellType::HALO;
                } else if (border) {
                    type = CellType::BORDER;
                } else {
                    type = CellType::INNER;
                }

                // position of lower left corner
                std::array<double, 3> position = {x * cellSize[0], y * cellSize[1], z * cellSize[2]};
                Cell c = Cell(cellSize, position, type, index, haloLocation);
                cells.push_back(c);

                // add to special cell ref. containers
                // we can do this in here because we reserved the size of the vector beforehand...
                if (type == CellType::HALO) {
                    haloCells.push_back(&cells[cells.size() - 1]);
                } else if (type == CellType::BORDER) {
                    borderCells.push_back(&cells[cells.size() - 1]);
                }

                SPDLOG_TRACE("Created new cell ({}, {}) (index: {}): {}", x, y, index, c.toString());
                index++;
            }
        }
    }

    // add particles to corresponding cells
    for (Particle &p : pc) {
        addParticle(p);
    }
}

/* iterators */
CellContainer::ContainerType::iterator CellContainer::begin() { return cells.begin(); }
CellContainer::ContainerType::iterator CellContainer::end() { return cells.end(); }
CellContainer::ContainerType::const_iterator CellContainer::begin() const { return cells.begin(); }
CellContainer::ContainerType::const_iterator CellContainer::end() const { return cells.end(); }

CellContainer::SpecialParticleIterator::SpecialParticleIterator(std::vector<Cell *>::iterator start,
                                                                std::vector<Cell *>::iterator end)
    : outerIt(start), outerEnd(end) {
    if (outerIt != outerEnd) {
        innerIt = (*outerIt)->getParticles().begin();
        innerEnd = (*outerIt)->getParticles().end();
        advance();
    }
}

Particle &CellContainer::SpecialParticleIterator::operator*() { return **innerIt; }
CellContainer::SpecialParticleIterator &CellContainer::SpecialParticleIterator::operator++() {
    ++innerIt;
    advance();
    return *this;
}
void CellContainer::SpecialParticleIterator::advance() {
    while (outerIt != outerEnd && innerIt == innerEnd) {
        ++outerIt;
        if (outerIt != outerEnd) {
            innerIt = (*outerIt)->getParticles().begin();
            innerEnd = (*outerIt)->getParticles().end();
        }
    }
}
bool CellContainer::SpecialParticleIterator::operator!=(const SpecialParticleIterator &other) const {
    return outerIt != other.outerIt || (outerIt != outerEnd && innerIt != other.innerIt);
}
bool CellContainer::SpecialParticleIterator::operator==(const SpecialParticleIterator &other) const {
    return !((*this) != other);
}
CellContainer::SpecialParticleIterator CellContainer::boundaryBegin() {
    return SpecialParticleIterator(borderCells.begin(), borderCells.end());
}
CellContainer::SpecialParticleIterator CellContainer::boundaryEnd() {
    return SpecialParticleIterator(borderCells.end(), borderCells.end());
}
CellContainer::SpecialParticleIterator CellContainer::haloBegin() {
    return SpecialParticleIterator(haloCells.begin(), haloCells.end());
}
CellContainer::SpecialParticleIterator CellContainer::haloEnd() {
    return SpecialParticleIterator(haloCells.end(), haloCells.end());
}

/* functionality */
Cell &CellContainer::operator[](size_t index) { return cells[index]; }
const Cell &CellContainer::operator[](size_t index) const { return cells[index]; }

int CellContainer::getCellIndex(const std::array<double, 3> &position) {
    std::array<int, 3> coords{0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        // prevent division by zero for 2D; could also simply check dim here if it was stored, but i want to keep it as
        // general as possible
        if (cellSize[i] == 0) {
            coords[i] = 0;
            continue;
        }
        // get coordinates based on position
        coords[i] = static_cast<int>(std::floor(position[i] / cellSize[i])); // +1 for halo offset
        if (coords[i] < 0 || coords[i] >= numCells[i]) {
            SPDLOG_DEBUG("Position {} is out of bounds!", ArrayUtils::to_string(position));
            return -1; // out of bounds
        }
    }
    int idx = coords[2] * numCells[0] * numCells[1] + coords[1] * numCells[0] + coords[0];
    SPDLOG_TRACE("Cell of position {} is at index {}: {}", ArrayUtils::to_string(position), idx, cells[idx].toString());
    return idx;
}

// add a particle to the appropriate cell
bool CellContainer::addParticle(Particle &p) {
    int cellIndex = getCellIndex(p.getX());
    if (cellIndex >= 0 && cellIndex < static_cast<int>(cells.size())) {
        p.setCellIndex(cellIndex);
        cells[cellIndex].addParticle(&p);
        SPDLOG_TRACE("Added particle {}", p.toString());
        return true;
    } else {
        SPDLOG_TRACE("Failed to add particle {}, could not find index in cell vector!", p.toString());
        return false;
    }
}

// remove a particle from a cell
// maybe add a check to see if p's cellIndex is -1?
void CellContainer::deleteParticle(Particle &p) {
    int cellIndex = p.getCellIndex();
    cells[cellIndex].removeParticle(&p);
    p.setCellIndex(-1);
    SPDLOG_TRACE("Removed particle from cell {}: {}", cellIndex, p.toString());
}

// move a particle (or not, idc)
bool CellContainer::moveParticle(Particle &p) {
    deleteParticle(p);
    return addParticle(p);
}

// get the (x, y, z) coordinates of a cell from its 1D index
std::array<int, 3> CellContainer::getVirtualCellCoordinates(int index) {
    int x = index % numCells[0];
    int y = (index / numCells[0]) % numCells[1];
    int z = cellSize[2] == 0 ? 0 : index / (numCells[0] * numCells[1]);
    return {x, y, z};
}
std::vector<Cell> &CellContainer::getCells() { return cells; }
std::vector<Cell *> &CellContainer::getBorderCells() { return borderCells; }
std::vector<Cell *> &CellContainer::getHaloCells() { return haloCells; }

int CellContainer::getOppositeNeighbor(int cellIndex, const std::vector<HaloLocation> &directions) {
    int idx = cellIndex;
    for (auto l : directions) {
        switch (l) {
        case HaloLocation::NORTH:
            idx -= numCells[0];
            SPDLOG_TRACE("North set, getting southern cell index...");
            break;
        case HaloLocation::SOUTH:
            idx += numCells[0];
            SPDLOG_TRACE("South set, getting northern cell index...");
            break;
        case HaloLocation::EAST:
            idx -= 1;
            SPDLOG_TRACE("East set, getting western cell index...");
            break;
        case HaloLocation::WEST:
            idx += 1;
            SPDLOG_TRACE("West set, getting eastern cell index...");
            break;
        default:
            SPDLOG_WARN("Not yet implemented! Come back later.\n");
            break;
        }
    }
    return idx;
}

std::array<double, 3> CellContainer::getMirrorPosition(const std::array<double, 3> &position, const Cell &from,
                                                       const Cell &to, int direction) {
    std::array<double, 3> posWithinCell = position - from.getX();
    double offset = from.getSize()[direction] - posWithinCell[direction];
    if (direction == 2) {
        // above or below
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + posWithinCell[1], to.getX()[2] + offset};
    } else if (direction == 1) {
        // north or south
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + offset, to.getX()[2] + posWithinCell[2]};
    } else {
        // east or west
        return {to.getX()[0] + offset, to.getX()[1] + posWithinCell[1], to.getX()[2] + posWithinCell[2]};
    }
}

// return indices of ALL neighbours (including itself)
std::vector<int> CellContainer::getNeighbors(int cellIndex) {
    std::vector<int> neighbors;
    std::array<int, 3> coords = getVirtualCellCoordinates(cellIndex);

    for (int dz = (cellSize[2] == 0 ? 0 : -1); dz <= (cellSize[2] == 0 ? 0 : 1); ++dz) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0 && dz == 0) {
                    neighbors.push_back(cellIndex);
                    continue;
                }
                std::array<int, 3> neighborCoords = {coords[0] + dx, coords[1] + dy, 0};
                if (neighborCoords[0] >= 0 && neighborCoords[0] < numCells[0] && neighborCoords[1] >= 0 &&
                    neighborCoords[1] < numCells[1]) {
                    int neighborIndex = neighborCoords[1] * numCells[0] + neighborCoords[0];
                    neighbors.push_back(neighborIndex);
                }
            }
        }
    }

    return neighbors;
}

const std::array<double, 3> &CellContainer::getDomainSize() { return domainSize; }
const std::array<double, 3> &CellContainer::getCellSize() { return cellSize; }
const std::array<size_t, 3> &CellContainer::getNumCells() { return numCells; }
double CellContainer::getCutoff() { return cutoff; }
ParticleContainer &CellContainer::getParticles() { return particles; }
const std::array<BoundaryCondition, 6> &CellContainer::getConditions() { return conditions; }
size_t CellContainer::size() const { return particles.size(); }
size_t CellContainer::activeSize() const { return particles.activeSize(); }
