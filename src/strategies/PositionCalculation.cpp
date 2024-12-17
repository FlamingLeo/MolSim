#include "PositionCalculation.h"
#include "BoundaryConditions.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <vector>

void calculateX(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *) {
    SPDLOG_TRACE("Calculating new position...");
    for (auto &p : particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));

        // store previous force, then reset force to 0
        // TODO gravity?
        p.setOldF(p.getF());
        p.setFToZero();
    }
}

void calculateX_LC(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *lc) {
    SPDLOG_TRACE("Calculating new position (linked cells)...");
    for (auto &p : particles) {
        if (!p.isActive())
            continue;

        // update position
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));

        // store previous force for velocity calculation, then reset force to 0
        p.setOldF(p.getF());
        p.setFToZero();

        // check particle index and potentially move it
        // if the particle somehow goes completely out of bounds, remove it to avoid issues
        int newIdx = lc->getCellIndex(p.getX());
        if (newIdx == -1) {
            SPDLOG_ERROR("Particle {} out of bounds! Removing...", p.toString());
            p.markInactive();
            p.setCellIndex(-1);
            continue;
        }
        if (newIdx != p.getCellIndex()) {
            SPDLOG_TRACE("Index mismatch (current: {}, expected: {}), moving...", p.getCellIndex(), newIdx);

            Cell &targetCell = (*lc)[newIdx];

            if(newIdx < lc->getNumCells()[0]){
                SPDLOG_DEBUG("We have particle in cell {} in lower halo {}", newIdx, p.toString());
            }


            if (handleHaloCell(p, targetCell, lc))
                continue;

            if (!lc->moveParticle(p)) {
                SPDLOG_ERROR("Cannot move particle {}!", p.toString());
                p.markInactive();
            }
        }
    }
}