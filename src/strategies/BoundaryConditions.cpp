#include "BoundaryConditions.h"
#include "objects/Cell.h"
#include "objects/CellContainer.h"
#include "objects/Particle.h"
#include "utils/CellUtils.h"
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

bool handleHaloCell(Particle &p, Cell &targetCell, CellContainer *lc) {
    // don't do anything if the cell is not a halo cell
    if (targetCell.getType() != CellType::HALO)
        return false;

    // determine boundary conditions, especially if the cell is a corner cell
    // see the report and presentation for worksheet 3 for a detailed explanation...
    auto [loc, bc] = determineBoundaryCondition(p, targetCell, lc);
    SPDLOG_DEBUG("Chose boundary condition {} for cell {}.", CellUtils::fromBoundaryCondition(bc),
                 targetCell.toString());

    // handle corresponding boundary condition(s)
    switch (bc) {
    case BoundaryCondition::OUTFLOW:
        handleOutflowCondition(p, targetCell, lc);
        return true;
    case BoundaryCondition::REFLECTIVE:
        handleReflectiveCondition(p, targetCell, loc, lc);
        return true;
    case BoundaryCondition::PERIODIC:
        handlePeriodicCondition(p, targetCell, lc);
        return true;
    default:
        return false;
    }
}

std::pair<HaloLocation, BoundaryCondition> determineBoundaryCondition(Particle &p, Cell &targetCell,
                                                                      CellContainer *lc) {
    // get cardinal direction(s) of halo cell
    // TODO: i'm still debating whether a bitmap would be faster...
    const std::vector<HaloLocation> &haloLocations = targetCell.getHaloLocation();

    // if there is more than one cardinal direction, the cell is a _corner_ halo cell
    if (haloLocations.size() > 1) {
        SPDLOG_DEBUG("Found multiple for (corner) cell {}...", targetCell.toString());
        HaloLocation location = targetCell.getCornerRegion(p);
        BoundaryCondition condition = lc->getConditions()[static_cast<int>(location)];
        return {location, condition};
    }

    // otherwise, just go with the first (and only) one
    HaloLocation location = haloLocations[0];
    BoundaryCondition condition = lc->getConditions()[static_cast<int>(location)];
    SPDLOG_DEBUG("Choosing boundary condition for cell {}: {} (cast: {})...", targetCell.toString(),
                 CellUtils::fromHalo(location), static_cast<int>(location));
    return {location, condition};
}

void handleOutflowCondition(Particle &p, Cell &targetCell, CellContainer *lc) {
    // outflow simply removes the particle from the simulation, no fancy case-handling here...
    SPDLOG_DEBUG("[outflow] Particle {} entered HALO cell ({}), deleting...", p.toString(),
                 CellUtils::fromHaloVec(targetCell.getHaloLocation()));
    lc->deleteParticle(p);
    p.markInactive();
}

void handleReflectiveCondition(Particle &p, Cell &fromCell, HaloLocation location, CellContainer *lc) {
    // get cell to reflect particle into
    int oppIdx = lc->getOppositeNeighbor(fromCell.getIndex(), location);
    Cell &toCell = (*lc)[oppIdx];

    // decide, which way to flip particle speed and position
    bool flipVert = (location == HaloLocation::NORTH) || (location == HaloLocation::SOUTH);
    bool flipHorizontal = (location == HaloLocation::EAST) || (location == HaloLocation::WEST);
    bool flipDepth = (location == HaloLocation::BELOW) || (location == HaloLocation::ABOVE);

    // reflect
    if (flipVert) {
        reflectParticle(p, fromCell, toCell, lc, 1);
        p.setV({p.getV()[0], -p.getV()[1], p.getV()[2]});
        SPDLOG_DEBUG("Flipped vertically: {}", p.toString());
    } else if (flipHorizontal) {
        reflectParticle(p, fromCell, toCell, lc, 0);
        p.setV({-p.getV()[0], p.getV()[1], p.getV()[2]});
        SPDLOG_DEBUG("Flipped horizontally: {}", p.toString());
    } else if (flipDepth) {
        reflectParticle(p, fromCell, toCell, lc, 2);
        p.setV({p.getV()[0], p.getV()[1], -p.getV()[2]});
        SPDLOG_DEBUG("Flipped depth-wise: {}", p.toString());
    }

    // handle further halo cell properties, if the particle is reflected inside another halo cell
    handleHaloCell(p, lc->getCells()[p.getCellIndex()], lc);
}

void handlePeriodicCondition(Particle &p, Cell &targetCell, CellContainer *lc) {
    // this function ought to be called after particles have made it into the halo cells and should be moved to the
    // opposite border cell
    const std::vector<HaloLocation> &haloLocations = targetCell.getHaloLocation();
    HaloLocation location;

    if (haloLocations.size() == 1) {
        location = haloLocations[0];
    } else if (haloLocations.size() > 1) {
        location = targetCell.getCornerRegion(p);
    } else {
        location = HaloLocation::NORTH; // just something so location is initialized
        SPDLOG_DEBUG("Could not determine direction for periodic conditions: {}", p.toString());
    }

    Cell &newCell = lc->getCells()[lc->getOppositeOfHalo(targetCell, location)];
    std::array<double, 3> inCell = {p.getX()[0] - targetCell.getX()[0], p.getX()[1] - targetCell.getX()[1],
                                    p.getX()[2] - targetCell.getX()[2]};
    std::array<double, 3> newPos = {newCell.getX()[0] + inCell[0], newCell.getX()[1] + inCell[1],
                                    newCell.getX()[2] + inCell[2]};

    // we set the particle's new position and move it to the proper cell
    p.setX(newPos);
    if (!lc->moveParticle(p)) {
        SPDLOG_ERROR("Invalid particle position! {}", p.toString());
        p.markInactive();
        return;
    }

    // if it was a corner halo, we need to recalculate boundary conditions because the particle still in another halo
    // cell
    if (haloLocations.size() > 1) {
        SPDLOG_DEBUG("Found another boundary in cell {} for particle {}", p.getCellIndex(), p.toString());
        handleHaloCell(p, lc->getCells()[p.getCellIndex()], lc);
    }
    SPDLOG_DEBUG("Moved periodic {} to cell {}.", p.toString(), p.getCellIndex());
}

void mirrorGhostParticles(CellContainer *lc) {
    std::vector<std::reference_wrapper<Cell>> borderCells = lc->getBorderCells();

    // we add references to the particles to the halo cells on the opposite side (sides if corner)
    for (auto bc : borderCells) {
        std::vector<BorderLocation> location = bc.get().getBorderLocation();

        // I'm starting to think it should be 1 even for 3D (imagine a cross section)
        if (location.size() > 1) {
            // At least 2 edges should be periodic for us to mirror in corner (intuition)
            int doCorner = 0;
            std::vector<BorderLocation> periodicBorders;
            for (auto direction : location) {
                if (lc->getConditions()[static_cast<int>(direction)] == BoundaryCondition::PERIODIC) {
                    periodicBorders.push_back(direction);
                    doCorner += 1;
                }
            }

            if (doCorner >= 2) {
                std::vector<int> corners = lc->getOppositeOfBorderCorner(bc, periodicBorders);
                // in every corner add the ghost particles
                for (auto corner : corners) {
                    for (auto &p : bc.get().getParticles()) {
                        lc->getCells()[corner].addParticle(p.get());
                        SPDLOG_DEBUG("Mirror in corner {} with corner {} and actual {}.", p.get().toString(), corner,
                                     p.get().getCellIndex());
                    }
                }
            }
        }

        // case for edges: you need to mirror across every edge
        for (auto direction : location) {
            // check whether we should be applying periodic boundary conditions in this direction
            if (lc->getConditions()[static_cast<int>(direction)] != BoundaryCondition::PERIODIC) {
                continue;
            }

            int haloIndex = lc->getOppositeOfBorder(bc, direction);

            for (auto &p : bc.get().getParticles()) {
                lc->getCells()[haloIndex].addParticle(p.get());
                SPDLOG_DEBUG("Mirror along edge {} in {} and in actual {}.", p.get().toString(), haloIndex,
                             p.get().getCellIndex());
            }
        }
    }
}

void deleteGhostParticles(CellContainer *lc) {
    // remove all particles (they are only ghost particles) from the halo cells, they should be empty
    for (auto &cell : lc->getHaloCells()) {
        cell.get().getParticles().clear();
    }
}

void reflectParticle(Particle &p, Cell &fromCell, Cell &toCell, CellContainer *lc, int direction) {
    p.setX(lc->getMirrorPosition(p.getX(), fromCell, toCell, direction));
    if (!lc->moveParticle(p)) {
        SPDLOG_ERROR("Error reflecting particle {}! from: {}, to: {}", p.toString(), fromCell.getIndex(),
                     toCell.getIndex());
        p.markInactive();
        return;
    }
    SPDLOG_DEBUG("Moved {} to cell {}.", p.toString(), p.getCellIndex());
}