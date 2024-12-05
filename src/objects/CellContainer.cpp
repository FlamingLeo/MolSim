#include "CellContainer.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

/* constructor */
CellContainer::CellContainer(const std::array<double, 3> &domainSize,
                             const std::array<BoundaryCondition, 6> &conditions, double cutoff,
                             ParticleContainer &particles, size_t dim)
    : domainSize{domainSize}, conditions{conditions}, cutoff{cutoff}, particles{particles} {
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
            numCells[i] = std::floor(domainSize[i] / cellSize[i]);
            cellSize[i] = domainSize[i] / numCells[i] ;
            numCells[i] += 2; // 2 halo cells
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

                // we don't care about which type of border it is, for now...
                bool border = dim == 3 ? (z == 1 || z == (numCells[2] - 2) || y == 1 || y == (numCells[1] - 2) ||
                                          x == 1 || x == (numCells[0] - 2))
                                       : (y == 1 || y == (numCells[1] - 2) || x == 1 || x == (numCells[0] - 2));
                CellType type = !haloLocation.empty() ? CellType::HALO : (border ? CellType::BORDER : CellType::INNER);

                // position of lower left corner
                std::array<double, 3> position = {x * cellSize[0], y * cellSize[1], z * cellSize[2]};
                cells.emplace_back(cellSize, position, type, index, haloLocation);

                // add to special cell ref. containers
                // we can do this in here because we reserved the size of the vector beforehand...
                if (type == CellType::HALO) {
                    haloCells.push_back(std::ref(cells[cells.size() - 1]));
                } else if (type == CellType::BORDER) {
                    borderCells.push_back(std::ref(cells[cells.size() - 1]));
                }

                SPDLOG_TRACE("Created new cell ({}, {}) (index: {})", x, y, index);
                index++;
            }
        }
    }

    // add particles to corresponding cells
    for (Particle &p : particles) {
        addParticle(p);
    }
}

/* iterators */
ParticleContainer::PairIterator CellContainer::beginPairs() { return particles.beginPairs(); }
ParticleContainer::PairIterator CellContainer::endPairs() { return particles.endPairs(); }
CellContainer::ContainerType::iterator CellContainer::begin() { return cells.begin(); }
CellContainer::ContainerType::iterator CellContainer::end() { return cells.end(); }
CellContainer::ContainerType::const_iterator CellContainer::begin() const { return cells.begin(); }
CellContainer::ContainerType::const_iterator CellContainer::end() const { return cells.end(); }

CellContainer::SpecialParticleIterator::SpecialParticleIterator(
    std::vector<std::reference_wrapper<Cell>>::iterator start, std::vector<std::reference_wrapper<Cell>>::iterator end)
    : outerIt(start), outerEnd(end) {
    if (outerIt != outerEnd) {
        innerIt = outerIt->get().begin();
        innerEnd = outerIt->get().end();
        advance();
    }
}

Particle &CellContainer::SpecialParticleIterator::operator*() { return *innerIt; }
CellContainer::SpecialParticleIterator &CellContainer::SpecialParticleIterator::operator++() {
    ++innerIt;
    advance();
    return *this;
}
void CellContainer::SpecialParticleIterator::advance() {
    while (outerIt != outerEnd && innerIt == innerEnd) {
        ++outerIt;
        if (outerIt != outerEnd) {
            innerIt = outerIt->get().begin();
            innerEnd = outerIt->get().end();
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
int CellContainer::getCellIndex(const std::array<double, 3> &position) {
    std::array<int, 3> coords{0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        // prevent division by zero for 2D; could also simply check dim here if it were stored.
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
    return idx;
}
bool CellContainer::addParticle(Particle &p) {
    int cellIndex = getCellIndex(p.getX());
    if (cellIndex >= 0 && cellIndex < static_cast<int>(cells.size())) {
        p.setCellIndex(cellIndex);
        cells[cellIndex].addParticle(p);
        SPDLOG_TRACE("Added particle {}", p.toString());
        return true;
    } else {
        SPDLOG_TRACE("Failed to add particle {}, could not find index in cell vector!", p.toString());
        return false;
    }
}
void CellContainer::deleteParticle(Particle &p) {
    int cellIndex = p.getCellIndex();
    assert(cellIndex != -1);
    cells[cellIndex].removeParticle(p);
    p.setCellIndex(-1);
    SPDLOG_TRACE("Removed particle from cell {}: {}", cellIndex, p.toString());
}
bool CellContainer::moveParticle(Particle &p) {
    deleteParticle(p);
    return addParticle(p);
}
void CellContainer::removeHaloCellParticles() {
    for (auto &p : particles) {
        if (p.isActive() && p.getCellIndex() != -1) {
            if (cells[p.getCellIndex()].getType() == CellType::HALO) {
                SPDLOG_TRACE("Found active halo particle, removing...");
                deleteParticle(p);
                p.markInactive();
            }
        }
    }
}
std::array<int, 3> CellContainer::getVirtualCellCoordinates(int index) const {
    int x = index % numCells[0];
    int y = (index / numCells[0]) % numCells[1];
    int z = cellSize[2] == 0 ? 0 : index / (numCells[0] * numCells[1]);
    return {x, y, z};
}
int CellContainer::getOppositeNeighbor(int cellIndex, const std::vector<HaloLocation> &directions) const {
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
                                                       const Cell &to, int direction) const {
    std::array<double, 3> posWithinCell = position - from.getX();
    double xOffset = from.getSize()[0] - posWithinCell[0];
    double yOffset = from.getSize()[1] - posWithinCell[1];
    double zOffset = from.getSize()[2] - posWithinCell[2];
    if (direction == 2) {
        // north, south, east or west
        return {to.getX()[0] + xOffset, to.getX()[1] + yOffset, to.getX()[2] + posWithinCell[2]};
    } else if (direction == 1) {
        // north or south
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + yOffset, to.getX()[2] + posWithinCell[2]};
    } else {
        // east or west
        return {to.getX()[0] + xOffset, to.getX()[1] + posWithinCell[1], to.getX()[2] + posWithinCell[2]};
    }
}
std::vector<int> CellContainer::getNeighbors(int cellIndex) const {
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

Cell &CellContainer::operator[](size_t index) { return cells[index]; }
const Cell &CellContainer::operator[](size_t index) const { return cells[index]; }
std::vector<Cell> &CellContainer::getCells() { return cells; }
const std::vector<Cell> &CellContainer::getCells() const { return cells; }
std::vector<std::reference_wrapper<Cell>> &CellContainer::getBorderCells() { return borderCells; }
const std::vector<std::reference_wrapper<Cell>> &CellContainer::getBorderCells() const { return borderCells; }
std::vector<std::reference_wrapper<Cell>> &CellContainer::getHaloCells() { return haloCells; }
const std::vector<std::reference_wrapper<Cell>> &CellContainer::getHaloCells() const { return haloCells; }
const std::array<double, 3> &CellContainer::getDomainSize() const { return domainSize; }
const std::array<double, 3> &CellContainer::getCellSize() const { return cellSize; }
const std::array<size_t, 3> &CellContainer::getNumCells() const { return numCells; }
const std::array<BoundaryCondition, 6> &CellContainer::getConditions() const { return conditions; }
double CellContainer::getCutoff() const { return cutoff; }
ParticleContainer &CellContainer::getParticles() { return particles; }
const ParticleContainer &CellContainer::getParticles() const { return particles; }
size_t CellContainer::size() const { return particles.size(); }
size_t CellContainer::activeSize() const { return particles.activeSize(); }

/* debug functions */
void CellContainer::printCellIndices() const {
    int maxIndex = cells.size() - 1;
    int width = 0;
    while (maxIndex > 0) {
        maxIndex /= 10;
        width++;
    }
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

            std::cout << std::setw(width) << i << " " << RST;
        }
        std::cout << "\n";
    }
}
void CellContainer::printCellContents() const {
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << BOLD_ON << "Cell " << i << ":\n";
        for (auto &p : cells[i].getParticles()) {
            std::cout << "\t" << p.get().toString() << "\n";
        }
    }
}