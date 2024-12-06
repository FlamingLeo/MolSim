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
    SPDLOG_DEBUG("Chose boundary condition {} for cell {}.", CellUtils::fromBoundaryCondition(bc),
                 targetCell.toString());

    switch (bc) {
    case BoundaryCondition::OUTFLOW:
        handleOutflowCondition(p, targetCell, lc);
        return true;

    case BoundaryCondition::REFLECTIVE:
        handleReflectiveCondition(p, targetCell, lc);
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

    SPDLOG_DEBUG("Choosing boundary condition for cell {}: {} (cast: {})...", targetCell.toString(),
                 CellUtils::fromHalo(haloLocations[0]), static_cast<int>(haloLocations[0]));
    return lc->getConditions()[static_cast<int>(haloLocations[0])];
}

void handleOutflowCondition(Particle &p, Cell &targetCell, CellContainer *lc) {
    SPDLOG_DEBUG("[outflow] Particle {} entered HALO cell ({}), deleting...", p.toString(),
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
        SPDLOG_DEBUG("Flipped vertically and horizontally: {}", p.toString());
    } else if (flipVert) {
        reflectParticle(p, fromCell, toCell, lc, 1);
        p.setV({p.getV()[0], -p.getV()[1], p.getV()[2]});
        SPDLOG_DEBUG("Flipped vertically: {}", p.toString());
    } else if (flipHorizontal) {
        reflectParticle(p, fromCell, toCell, lc, 0);
        p.setV({-p.getV()[0], p.getV()[1], p.getV()[2]});
        SPDLOG_DEBUG("Flipped horizontally: {}", p.toString());
    }
}

void reflectParticle(Particle &p, Cell &fromCell, Cell &toCell, CellContainer *lc, int dimension) {
    p.setX(lc->getMirrorPosition(p.getX(), fromCell, toCell, dimension));
    if (!lc->moveParticle(p)) {
        p.markInactive();
        return;
    }
    SPDLOG_DEBUG("Moved to cell {}.", p.getCellIndex());
}