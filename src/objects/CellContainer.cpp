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
// TODO: (re)make this compatible with 3rd dimension...
CellContainer::CellContainer(const std::array<double, 3> &domainSize,
                             const std::array<BoundaryCondition, 6> &conditions, double cutoff, ParticleContainer &pc)
    : domainSize{domainSize}, conditions{conditions}, cutoff{cutoff}, particles{pc} {
    // check that domain size and cutoff are initialized
    if (cutoff == INF)
        CLIUtils::error("Cutoff radius not initialized!");
    if (domainSize[0] == INF || domainSize[1] == INF || domainSize[2] == INF)
        CLIUtils::error("Domain size not initialized!");

    SPDLOG_TRACE("Generating CellContainer with domain size {} and cutoff radius {}", ArrayUtils::to_string(domainSize),
                 cutoff);

    // determining cell size
    cellSize = {0, 0, 0};
    for (size_t i = 0; i < 2; i++) {
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
    size_t totalNumCells = numCells[0] * numCells[1];
    cells.reserve(totalNumCells);
    SPDLOG_DEBUG("Reserved space for {} cells (X: {}, Y: {}).", totalNumCells, numCells[0], numCells[1]);

    // creating cells
    size_t index = 0;
    for (int y = 0; y < numCells[1]; y++) {
        for (int x = 0; x < numCells[0]; x++) {
            // set type of cell
            bool northHalo = (y == numCells[1] - 1);
            bool southHalo = (y == 0);
            bool westHalo = (x == 0);
            bool eastHalo = (x == numCells[0] - 1);

            std::vector<HaloLocation> haloLocation;
            if (northHalo)
                haloLocation.push_back(HaloLocation::NORTH);
            if (southHalo)
                haloLocation.push_back(HaloLocation::SOUTH);
            if (westHalo)
                haloLocation.push_back(HaloLocation::WEST);
            if (eastHalo)
                haloLocation.push_back(HaloLocation::EAST);

            bool border = (y == 1 || y == (numCells[1] - 2) || x == 1 || x == (numCells[0] - 2));
            CellType type;
            if (!haloLocation.empty()) {
                type = CellType::HALO;
            } else if (border) {
                type = CellType::BORDER;
            } else {
                type = CellType::INNER;
            }

            // position of lower left corner
            std::array<double, 3> position = {x * cellSize[0], y * cellSize[1], 0};
            Cell c = Cell(cellSize, position, type, index, haloLocation);
            cells.push_back(c);

            // add to special cell ref. containers
            if (type == CellType::HALO) {
                haloCells.push_back(&c);
            } else if (type == CellType::BORDER) {
                borderCells.push_back(&c);
            }

            SPDLOG_TRACE("Created new cell ({}, {}) (index: {}): {}", x, y, index, c.toString());
            index++;
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
    : cellIt(start), cellEnd(end) {
    if (cellIt != cellEnd) {
        particleIt = (*cellIt)->getParticles().begin();
        particleEnd = (*cellIt)->getParticles().end();
        advanceToNextCell();
    }
}
void CellContainer::SpecialParticleIterator::advanceToNextCell() {
    while (cellIt != cellEnd && particleIt == particleEnd) {
        ++cellIt;
        if (cellIt != cellEnd) {
            particleIt = (*cellIt)->getParticles().begin();
            particleEnd = (*cellIt)->getParticles().end();
        }
    }
}
Particle &CellContainer::SpecialParticleIterator::operator*() { return **particleIt; }
CellContainer::SpecialParticleIterator &CellContainer::SpecialParticleIterator::operator++() {
    ++particleIt;
    advanceToNextCell();
    return *this;
}
bool CellContainer::SpecialParticleIterator::operator!=(const SpecialParticleIterator &other) const {
    return cellIt != other.cellIt || (cellIt != cellEnd && particleIt != other.particleIt);
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
    for (int i = 0; i < 2; ++i) {
        // get coordinates based on position
        coords[i] = static_cast<int>(std::floor(position[i] / cellSize[i])); // +1 for halo offset
        if (coords[i] < 0 || coords[i] >= numCells[i]) {
            SPDLOG_DEBUG("Position {} is out of bounds!", ArrayUtils::to_string(position));
            return -1; // out of bounds
        }
    }
    int idx = coords[1] * numCells[0] + coords[0];
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
    return {x, y, 0};
}

std::vector<Cell> &CellContainer::getCells() { return cells; }

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
            SPDLOG_DEBUG("Not yet implemented!\n");
            break;
        }
    }
    return idx;
}

std::array<double, 3> CellContainer::getMirrorPosition(const std::array<double, 3> &position, const Cell &from,
                                                       const Cell &to, int direction) {
    std::array<double, 3> posWithinCell = position - from.getX();
    if (direction == 1) {
        // north or south
        double yOffset = from.getSize()[1] - posWithinCell[1];
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + yOffset, to.getX()[2] + posWithinCell[2]};
    } else {
        // east or west
        double xOffset = from.getSize()[0] - posWithinCell[0];
        return {to.getX()[0] + xOffset, to.getX()[1] + posWithinCell[1], to.getX()[2] + posWithinCell[2]};
    }
}

// return indices of ALL neighbours (including itself)
std::vector<int> CellContainer::getNeighbors(int cellIndex) {
    std::vector<int> neighbors;
    std::array<int, 3> coords = getVirtualCellCoordinates(cellIndex);

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
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

    return neighbors;
}

const std::array<double, 3> &CellContainer::getDomainSize() { return domainSize; }
const std::array<double, 3> &CellContainer::getCellSize() { return cellSize; }
const std::array<size_t, 3> &CellContainer::getNumCells() { return numCells; }
double CellContainer::getCutoff() { return cutoff; }
ParticleContainer &CellContainer::getParticles() { return particles; }
const std::array<BoundaryCondition, 6> &CellContainer::getConditions() { return conditions; }

void CellContainer::printCellIndices() {
#define RED "\e[0;31m"
#define YEL "\e[0;33m"
#define GRN "\e[0;32m"
#define RST "\e[0m"
    for (int row = cells.size() / numCells[0] - 1; row >= 0; row--) {
        for (int col = 0; col < numCells[0]; col++) {
            int i = row * numCells[0] + col;
            CellType type = cells[i].getType();
            switch (type) {
            case CellType::HALO:
                std::cout << RED;
                break;
            case CellType::BORDER:
                std::cout << YEL;
                break;
            case CellType::INNER:
                std::cout << GRN;
                break;
            }
            std::cout << i
                      << (type == CellType::HALO ? ("(" + CellUtils::fromHaloVec(cells[i].getHaloLocation()) + ")")
                                                 : "")
                      << " ";
            std::cout << RST;
        }
        std::cout << "\n";
    }
}

void CellContainer::printCellContents() {
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << BOLD_ON << "Cell " << i << ":\n";
        for (auto *p : cells[i].getParticles()) {
            std::cout << "\t" << p->toString() << "\n";
        }
    }
}

ParticleContainer CellContainer::reconstructContainer() {
    ParticleContainer pc;
    pc.reserve(particles.size());
    for (size_t i = 0; i < cells.size(); ++i) {
        for (auto *p : cells[i].getParticles()) {
            pc.addParticle(*p);
        }
    }
    SPDLOG_TRACE("Generated new ParticleContainer with size {}.", pc.size());
    return pc;
}