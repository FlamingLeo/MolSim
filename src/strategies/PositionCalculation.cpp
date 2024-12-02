#include "PositionCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <vector>

void calculateX(ParticleContainer &particles, double delta_t, CellContainer *) {
    for (auto &p : particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));

        // set previous f for each particle and reinitialize to zero for the upcoming force calculation
        p.setOldF(p.getF());
        p.setFToZero();
    }
}

void calculateX_LC(ParticleContainer &particles, double delta_t, CellContainer *lc) {
    for (auto &p : particles) {
        if (!p.isActive())
            continue;

        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
        p.setFToZero();

        // check particle index and potentially move it
        int newIdx = lc->getCellIndex(p.getX());
        if (newIdx != p.getCellIndex()) {
            SPDLOG_DEBUG("Index mismatch (current: {}, expected: {}), moving...", p.getCellIndex(), newIdx);
            Cell &from = (*lc)[newIdx];

            // check boundary conditions if particle enters a halo cell
            if (from.getType() == CellType::HALO) {
                BoundaryCondition bc;
                const std::vector<HaloLocation> &hl = from.getHaloLocation();

                // if the cell has more than one entry in the halo vector, it's a corner halo cell
                // thus, we need to see which boundary condition takes precedence
                // otherwise, we simply get the condition from the vector
                if (hl.size() > 1) {
                    SPDLOG_DEBUG("Found multiple for Cell {}", from.toString());
                    bc = lc->getConditions()[static_cast<int>(from.getCornerRegion(p))];
                } else {
                    SPDLOG_DEBUG("Choosing boundary condition for cell {}: {} (cast: {})...", from.toString(),
                                 CellUtils::fromHalo(hl[0]), static_cast<int>(hl[0]));
                    bc = lc->getConditions()[static_cast<int>(hl[0])];
                }
                SPDLOG_DEBUG("Chose boundary condition {} for cell {}.", CellUtils::fromBoundaryCondition(bc),
                             from.toString());

                // based on chosen boundary condition, perform particle update
                switch (bc) {
                case BoundaryCondition::OUTFLOW: {
                    SPDLOG_DEBUG("[outflow] Particle {} entered HALO cell ({}), deleting...", p.toString(),
                                 CellUtils::fromHaloVec(hl));
                    lc->deleteParticle(p);
                    p.markInactive();
                    continue;
                } break;
                case BoundaryCondition::REFLECTIVE: {
                    int oppIdx = lc->getOppositeNeighbor(newIdx, hl);
                    SPDLOG_DEBUG("[reflective] Particle {} entered HALO cell {} ({}), reflecting to {}...",
                                 p.toString(), newIdx, CellUtils::fromHaloVec(hl), oppIdx);
                    Cell &to = (*lc)[oppIdx];
                    bool flipVert = VEC_CONTAINS(hl, HaloLocation::NORTH) || VEC_CONTAINS(hl, HaloLocation::SOUTH);
                    bool flipHorizontal = VEC_CONTAINS(hl, HaloLocation::EAST) || VEC_CONTAINS(hl, HaloLocation::WEST);
                    /* for 3D calculations, there will be an additional flipUpDown boolean here... */
                    if (flipVert && flipHorizontal) {
                        p.setX(lc->getMirrorPosition(p.getX(), from, to, 2));
                        if (!lc->moveParticle(p)) {
                            p.markInactive();
                            continue;
                        }
                        SPDLOG_DEBUG("Moved to cell {}.", p.getCellIndex());
                        p.setV({-p.getV()[0], -p.getV()[1], p.getV()[2]});
                        SPDLOG_DEBUG("Flipped vertically and horizontally: {}", p.toString());
                    } else if (flipVert) {
                        p.setX(lc->getMirrorPosition(p.getX(), from, to, 1));
                        if (!lc->moveParticle(p)) {
                            p.markInactive();
                            continue;
                        }
                        SPDLOG_DEBUG("Moved to cell {}.", p.getCellIndex());
                        p.setV({p.getV()[0], -p.getV()[1], p.getV()[2]});
                        SPDLOG_DEBUG("Flipped vertically: {}", p.toString());
                    } else if (flipHorizontal) {
                        p.setX(lc->getMirrorPosition(p.getX(), from, to, 0));
                        if (!lc->moveParticle(p)) {
                            p.markInactive();
                            continue;
                        }
                        SPDLOG_DEBUG("Moved to cell {}.", p.getCellIndex());
                        p.setV({-p.getV()[0], p.getV()[1], p.getV()[2]});
                        SPDLOG_DEBUG("Flipped horizontally: {}", p.toString());
                    }
                    continue;
                } break;
                }
            }

            // if the particle somehow leaves the domain, deactivate it
            if (!lc->moveParticle(p))
                p.markInactive();
        }
    }
}