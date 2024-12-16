#include "BoundaryConditions.h"
#include "objects/Cell.h"
#include "objects/CellContainer.h"
#include "objects/Particle.h"
#include "utils/CellUtils.h"
#include <spdlog/spdlog.h>
#include <vector>

bool handleHaloCell(Particle &p, Cell &targetCell, CellContainer *lc) {
    if (targetCell.getType() != CellType::HALO)
        return false;

    BoundaryCondition bc = determineBoundaryCondition(p, targetCell, lc);
    SPDLOG_TRACE("Chose boundary condition {} for cell {}.", CellUtils::fromBoundaryCondition(bc),
                 targetCell.toString());

    switch (bc) {
    case BoundaryCondition::OUTFLOW:
        handleOutflowCondition(p, targetCell, lc);
        return true;

    case BoundaryCondition::REFLECTIVE:
        handleReflectiveCondition(p, targetCell, lc);
        return true;

    case BoundaryCondition::PERIODIC:
        handlePeriodicCondition(p, targetCell, lc);
        return true;

    default:
        return false;
    }
}

BoundaryCondition determineBoundaryCondition(Particle &p, Cell &targetCell, CellContainer *lc) {
    const std::vector<HaloLocation> &haloLocations = targetCell.getHaloLocation();
    if (haloLocations.size() > 1) {
        SPDLOG_DEBUG("Found multiple for Cell {}", targetCell.toString());
        return lc->getConditions()[static_cast<int>(targetCell.getCornerRegion(p))];
    }

    SPDLOG_TRACE("Choosing boundary condition for cell {}: {} (cast: {})...", targetCell.toString(),
                 CellUtils::fromHalo(haloLocations[0]), static_cast<int>(haloLocations[0]));
    return lc->getConditions()[static_cast<int>(haloLocations[0])];
}

void handleOutflowCondition(Particle &p, Cell &targetCell, CellContainer *lc) {
    SPDLOG_TRACE("[outflow] Particle {} entered HALO cell ({}), deleting...", p.toString(),
                 CellUtils::fromHaloVec(targetCell.getHaloLocation()));
    lc->deleteParticle(p);
    p.markInactive();
}

void handleReflectiveCondition(Particle &p, Cell &fromCell, CellContainer *lc) {
    const std::vector<HaloLocation> &haloLocations = fromCell.getHaloLocation();
    int oppIdx = lc->getOppositeNeighbor(fromCell.getIndex(), haloLocations);
    Cell &toCell = (*lc)[oppIdx];

    bool flipVert =
        VEC_CONTAINS(haloLocations, HaloLocation::NORTH) || VEC_CONTAINS(haloLocations, HaloLocation::SOUTH);
    bool flipHorizontal =
        VEC_CONTAINS(haloLocations, HaloLocation::EAST) || VEC_CONTAINS(haloLocations, HaloLocation::WEST);

    if (flipVert && flipHorizontal) {
        reflectParticle(p, fromCell, toCell, lc, 2);
        p.setV({-p.getV()[0], -p.getV()[1], p.getV()[2]});
        SPDLOG_TRACE("Flipped vertically and horizontally: {}", p.toString());
    } else if (flipVert) {
        reflectParticle(p, fromCell, toCell, lc, 1);
        p.setV({p.getV()[0], -p.getV()[1], p.getV()[2]});
        SPDLOG_TRACE("Flipped vertically: {}", p.toString());
    } else if (flipHorizontal) {
        reflectParticle(p, fromCell, toCell, lc, 0);
        p.setV({-p.getV()[0], p.getV()[1], p.getV()[2]});
        SPDLOG_TRACE("Flipped horizontally: {}", p.toString());
    }
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
    SPDLOG_TRACE("Moved periodic {} to cell {}.", p.toString(), p.getCellIndex());
}

void mirrorGhostParticles(CellContainer *lc) {
    std::vector<std::reference_wrapper<Cell>> borderCells = lc->getBorderCells();

    // we add to the halo cells on the opposite side (sides if corner) references to the particles
    for (auto bc : borderCells) {
        std::vector<BorderLocation> location = bc.get().getBorderLocation();
        // special case for corners (2D) -- should be changed to >2 for 3D
        if (location.size() > 1) {
            // if not all concerned edges are periodic, don't mirror corner (intuitively this is how it seems it should
            // be to me)
            bool doCorner = true;
            for (auto direction : location) {
                if (lc->getConditions()[directionLookUp(direction)] != BoundaryCondition::PERIODIC) {
                    doCorner = false;
                }
            }

            if (doCorner) {
                std::vector<int> corners = lc->getOppositeOfBorderCorner(bc, location);
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
            if (lc->getConditions()[directionLookUp(direction)] != BoundaryCondition::PERIODIC) {
                continue;
            }

            int haloIndex = lc->getOppositeOfBorder(bc, direction);

            for (auto &p : bc.get().getParticles()) {
                lc->getCells()[haloIndex].addParticle(p.get());
                SPDLOG_TRACE("Mirror along edge {} in {} and in actual {}.", p.get().toString(), haloIndex,
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

void reflectParticle(Particle &p, Cell &fromCell, Cell &toCell, CellContainer *lc, int dimension) {
    p.setX(lc->getMirrorPosition(p.getX(), fromCell, toCell, dimension));
    if (!lc->moveParticle(p)) {
        SPDLOG_ERROR("Error reflecting particle {}! from: {}, to: {}", p.toString(), fromCell.getIndex(), toCell.getIndex());
        p.markInactive();
        return;
    }
    SPDLOG_TRACE("Moved {} to cell {}.", p.toString(), p.getCellIndex());
}

// TODO replace this with enum cast
int directionLookUp(BorderLocation location) {
    switch (location) {
    case BorderLocation::NORTH:
        return 0;
    case BorderLocation::SOUTH:
        return 1;
    case BorderLocation::WEST:
        return 2;
    case BorderLocation::EAST:
        return 3;
    case BorderLocation::ABOVE:
        return 4;
    case BorderLocation::BELOW:
        return 5;
    }
    return -1;
}