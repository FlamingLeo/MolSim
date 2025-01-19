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

#ifndef NDEBUG
#define PRINT_CELL_INDICES() printCellIndices()
#define PRINT_CELL_CONTENTS() printCellContents()
#else
#define PRINT_CELL_INDICES() (void)0
#define PRINT_CELL_CONTENTS() (void)0
#endif

/* constructor and destructor */
CellContainer::CellContainer(const std::array<double, 3> &domainSize,
                             const std::array<BoundaryCondition, 6> &conditions, double cutoff,
                             ParticleContainer &particles, size_t dim)
    : domainSize{domainSize}, conditions{conditions}, cutoff{cutoff}, particles{particles}, dim{dim} {
    // check correct dimensions (could probably be a boolean instead...)
    if (dim < 2 || dim > 3)
        CLIUtils::error("Invalid cell container dimensions! (must be 2 or 3)", StringUtils::fromNumber(dim));

    // check that domain size and cutoff are initialized
    if (cutoff == INFINITY)
        CLIUtils::error("Cutoff radius not initialized!");
    if (domainSize[0] == INFINITY || domainSize[1] == INFINITY || domainSize[2] == INFINITY)
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

    // reserve space for all cells and cell locks
    size_t totalNumCells = numCells[0] * numCells[1] * numCells[2];
    cells.reserve(totalNumCells);
    cellLocks.reserve(totalNumCells);
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

                bool aboveBorder = z == (numCells[2] - 2);
                bool belowBorder = z == 1;
                bool northBorder = y == (numCells[1] - 2);
                bool southBorder = y == 1;
                bool westBorder = x == 1;
                bool eastBorder = x == (numCells[0] - 2);

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

                std::vector<BorderLocation> borderLocation;
                if (northBorder)
                    borderLocation.push_back(BorderLocation::NORTH);
                if (southBorder)
                    borderLocation.push_back(BorderLocation::SOUTH);
                if (westBorder)
                    borderLocation.push_back(BorderLocation::WEST);
                if (eastBorder)
                    borderLocation.push_back(BorderLocation::EAST);
                if (dim == 3) {
                    if (aboveBorder)
                        borderLocation.push_back(BorderLocation::ABOVE);
                    if (belowBorder)
                        borderLocation.push_back(BorderLocation::BELOW);
                }

                // this should be deleted and borderLocation condition added, but I'm afraid to break everything (who
                // wrote this? - in any case I agree)
                // we don't care about which type of border it is, for now...
                bool border = dim == 3 ? (z == 1 || z == (numCells[2] - 2) || y == 1 || y == (numCells[1] - 2) ||
                                          x == 1 || x == (numCells[0] - 2))
                                       : (y == 1 || y == (numCells[1] - 2) || x == 1 || x == (numCells[0] - 2));
                CellType type = !haloLocation.empty() ? CellType::HALO : (border ? CellType::BORDER : CellType::INNER);

                // position of lower left corner
                std::array<double, 3> position = {x * cellSize[0], y * cellSize[1], z * cellSize[2]};
                cells.emplace_back(cellSize, position, type, index, haloLocation, borderLocation);
                calculateNeighbors(cells.size() - 1);

                // add to cell ref. containers
                // we can do this in here because we reserved the size of the vector beforehand...
                if (type == CellType::HALO) {
                    haloCells.push_back(std::ref(cells[cells.size() - 1]));
                } else if (type == CellType::BORDER) {
                    borderCells.push_back(std::ref(cells[cells.size() - 1]));
                    iterableCells.push_back(std::ref(cells[cells.size() - 1]));
                } else {
                    iterableCells.push_back(std::ref(cells[cells.size() - 1]));
                }

                SPDLOG_TRACE("Created new cell ({}, {}) (index: {})", x, y, index);

                // initialize corresponding cell lock
                omp_init_lock(&cellLocks[index]);

                index++;
            }
        }
    }

    // add particles to corresponding cells
    for (Particle &p : particles) {
        addParticle(p);
    }

    // debug print
    PRINT_CELL_INDICES();
    PRINT_CELL_CONTENTS();
}

CellContainer::~CellContainer() {
    for (size_t i = 0; i < cellLocks.size(); ++i) {
        omp_destroy_lock(&cellLocks[i]);
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
    int cellIndex = p.getCellIndex() == -1 ? getCellIndex(p.getX()) : p.getCellIndex();
    if (cellIndex >= 0 && cellIndex < static_cast<int>(cells.size())) {
        p.setCellIndex(cellIndex);
        omp_set_lock(&cellLocks[cellIndex]);
        cells[cellIndex].addParticle(p);
        omp_unset_lock(&cellLocks[cellIndex]);
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
    omp_set_lock(&cellLocks[cellIndex]);
    cells[cellIndex].removeParticle(p);
    omp_unset_lock(&cellLocks[cellIndex]);
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
int CellContainer::getOppositeNeighbor(int cellIndex, HaloLocation direction) const {
    switch (direction) {
    case HaloLocation::NORTH:
        SPDLOG_TRACE("North set, getting southern cell index...");
        return cellIndex - numCells[0];
    case HaloLocation::SOUTH:
        SPDLOG_TRACE("South set, getting northern cell index...");
        return cellIndex + numCells[0];
        break;
    case HaloLocation::EAST:
        SPDLOG_TRACE("East set, getting western cell index...");
        return cellIndex - 1;
        break;
    case HaloLocation::WEST:
        SPDLOG_TRACE("West set, getting eastern cell index...");
        return cellIndex + 1;
        break;
    case HaloLocation::ABOVE:
        SPDLOG_TRACE("Above set, getting below cell index...");
        if (dim == 3) {
            return cellIndex - numCells[0] * numCells[1];
        } else {
            return -1;
        }
        break;
    case HaloLocation::BELOW:
        SPDLOG_TRACE("Below set, getting above cell index...");
        if (dim == 3) {
            return cellIndex + numCells[0] * numCells[1];
        } else {
            return -1;
        }
        break;
    default:
        SPDLOG_WARN("Not yet implemented! Come back later.\n");
        return -1;
    }
}
std::array<double, 3> CellContainer::getMirrorPosition(const std::array<double, 3> &position, const Cell &from,
                                                       const Cell &to, int direction) const {
    std::array<double, 3> posWithinCell = position - from.getX();
    double xOffset = from.getSize()[0] - posWithinCell[0];
    double yOffset = from.getSize()[1] - posWithinCell[1];
    double zOffset = from.getSize()[2] - posWithinCell[2];
    if (direction == 2) {
        // above or below
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + posWithinCell[1], to.getX()[2] + zOffset};
    } else if (direction == 1) {
        // north or south
        return {to.getX()[0] + posWithinCell[0], to.getX()[1] + yOffset, to.getX()[2] + posWithinCell[2]};
    } else {
        // east or west
        return {to.getX()[0] + xOffset, to.getX()[1] + posWithinCell[1], to.getX()[2] + posWithinCell[2]};
    }
}

void CellContainer::calculateNeighbors(int cellIndex) {
    std::array<int, 3> coords = getVirtualCellCoordinates(cellIndex);
    // here we check whether we have a 3rd dimension
    for (int dz = (cellSize[2] == 0 ? 0 : -1); dz <= (cellSize[2] == 0 ? 0 : 1); ++dz) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0 && dz == 0) {
                    // cell itself is also a neighbor
                    cells[cellIndex].getNeighbors().push_back(cellIndex);
                    continue;
                }
                // coords[2] + dz = 0 if in 2D
                std::array<int, 3> neighborCoords = {coords[0] + dx, coords[1] + dy, coords[2] + dz};
                if (neighborCoords[0] >= 0 && neighborCoords[0] < numCells[0] && neighborCoords[1] >= 0 &&
                    neighborCoords[1] < numCells[1] && neighborCoords[2] >= 0 && neighborCoords[2] < numCells[2]) {
                    int neighborIndex = neighborCoords[2] * numCells[1] * numCells[0] +
                                        neighborCoords[1] * numCells[0] + neighborCoords[0];
                    cells[cellIndex].getNeighbors().push_back(neighborIndex);
                }
            }
        }
    }
}

const std::vector<int> &CellContainer::getNeighbors(int cellIndex) const { return cells[cellIndex].getNeighbors(); }
std::vector<std::reference_wrapper<Cell>> CellContainer::getNeighborCells(int cellIndex) {
    std::vector<std::reference_wrapper<Cell>> neighbors;
    for (size_t idx : getNeighbors(cellIndex)) {
        neighbors.push_back(std::ref(cells[idx]));
    }
    return neighbors;
}

int CellContainer::getOppositeOfHalo(const Cell &from, HaloLocation location) {
    // coincidentally works just as well for getting the opposite halo cell for a border cell; currently in 2D
    int cellIndex = from.getIndex();
    if (location == HaloLocation::NORTH) {
        return cellIndex - numCells[0] * (numCells[1] - 2);
    } else if (location == HaloLocation::SOUTH) {
        return cellIndex + numCells[0] * (numCells[1] - 2);
    } else if (location == HaloLocation::WEST) {
        return cellIndex + (numCells[0] - 2);
    } else if (location == HaloLocation::EAST) {
        return cellIndex - (numCells[0] - 2);
    } else if (location == HaloLocation::BELOW) {
        return cellIndex + numCells[0] * numCells[1] * (numCells[2] - 2);
    } else if (location == HaloLocation::ABOVE) {
        return cellIndex - numCells[0] * numCells[1] * (numCells[2] - 2);
    }
    return -1;
}

int CellContainer::getOppositeOfBorder(const Cell &from, BorderLocation location) {
    // currently in 2D
    int cellIndex = from.getIndex();
    if (location == BorderLocation::NORTH) {
        return cellIndex - numCells[0] * (numCells[1] - 2);
    } else if (location == BorderLocation::SOUTH) {
        return cellIndex + numCells[0] * (numCells[1] - 2);
    } else if (location == BorderLocation::WEST) {
        return cellIndex + (numCells[0] - 2);
    } else if (location == BorderLocation::EAST) {
        return cellIndex - (numCells[0] - 2);
    } else if (location == BorderLocation::BELOW) {
        return cellIndex + numCells[0] * numCells[1] * (numCells[2] - 2);
    } else if (location == BorderLocation::ABOVE) {
        return cellIndex - numCells[0] * numCells[1] * (numCells[2] - 2);
    }
    return -1;
}

std::vector<int> CellContainer::getOppositeOfBorderCorner(const Cell &from, std::vector<BorderLocation> &locations) {
    // switched to 3D, should still work in 2D
    std::vector<int> ghostCorners;
    for (auto &pair : getBorderCombinations(locations)) {
        int cellIndex = from.getIndex();
        for (auto &loc : pair) {
            if (loc == BorderLocation::NORTH) {
                cellIndex = cellIndex - numCells[0] * (numCells[1] - 2);
            } else if (loc == BorderLocation::SOUTH) {
                cellIndex = cellIndex + numCells[0] * (numCells[1] - 2);
            } else if (loc == BorderLocation::WEST) {
                cellIndex = cellIndex + (numCells[0] - 2);
            } else if (loc == BorderLocation::EAST) {
                cellIndex = cellIndex - (numCells[0] - 2);
            } else if (loc == BorderLocation::ABOVE) {
                cellIndex = cellIndex - numCells[0] * numCells[1] * (numCells[2] - 2);
            } else if (loc == BorderLocation::BELOW) {
                cellIndex = cellIndex + numCells[0] * numCells[1] * (numCells[2] - 2);
            }
        }
        ghostCorners.push_back(cellIndex);
    }
    return ghostCorners;
}

std::vector<std::vector<BorderLocation>> CellContainer::getBorderCombinations(std::vector<BorderLocation> &locations) {
    std::vector<std::vector<BorderLocation>> pairs;
    int n = locations.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            pairs.push_back({locations[i], locations[j]});
        }
    }

    // if we have a triple corner, I have come to the conclusion we should also mirror across all 3 dimensions
    // it's just intuition tho, I wouldn't stake my life on it
    if (locations.size() == 3) {
        pairs.push_back(locations);
    }

    return pairs;
}

Cell &CellContainer::operator[](size_t index) { return cells[index]; }
const Cell &CellContainer::operator[](size_t index) const { return cells[index]; }
std::vector<Cell> &CellContainer::getCells() { return cells; }
const std::vector<Cell> &CellContainer::getCells() const { return cells; }
std::vector<std::reference_wrapper<Cell>> &CellContainer::getBorderCells() { return borderCells; }
const std::vector<std::reference_wrapper<Cell>> &CellContainer::getBorderCells() const { return borderCells; }
std::vector<std::reference_wrapper<Cell>> &CellContainer::getHaloCells() { return haloCells; }
const std::vector<std::reference_wrapper<Cell>> &CellContainer::getHaloCells() const { return haloCells; }
std::vector<std::reference_wrapper<Cell>> &CellContainer::getIterableCells() { return iterableCells; }
const std::vector<std::reference_wrapper<Cell>> &CellContainer::getIterableCells() const { return iterableCells; }
const std::array<double, 3> &CellContainer::getDomainSize() const { return domainSize; }
const std::array<double, 3> &CellContainer::getCellSize() const { return cellSize; }
const std::array<size_t, 3> &CellContainer::getNumCells() const { return numCells; }
const std::array<BoundaryCondition, 6> &CellContainer::getConditions() const { return conditions; }
double CellContainer::getCutoff() const { return cutoff; }
size_t CellContainer::getDim() const { return dim; }
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
        std::cout << BOLD_ON << "Cell " << i << "([" << cells[i].getX()[0] << ", " << cells[i].getX()[1] << ", "
                  << cells[i].getX()[2] << "] - [" << (cells[i].getX()[0] + cells[i].getSize()[0]) << ", "
                  << (cells[i].getX()[1] + cells[i].getSize()[1]) << ", "
                  << (cells[i].getX()[2] + cells[i].getSize()[2]) << "]): " << BOLD_OFF << "\n";
        for (auto &p : cells[i].getParticles()) {
            std::cout << "\t" << p.get().toString() << "\n";
        }
    }
    std::cout << BOLD_OFF;
}