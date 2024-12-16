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

        // set previous f for each particle and reinitialize using gravitational force for the upcoming force
        // calculation
        p.setOldF(p.getF());
        std::array<double, 3> gravity = {0.0, p.getM() * g_grav, 0.0};
        p.setF(gravity);
    }
}

void calculateX_LC(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *lc) {
    SPDLOG_TRACE("Calculating new position (linked cells)...");
    for (auto &p : particles) {
        if (!p.isActive())
            continue;

        // add gravitational force
        // std::array<double, 3> gravity = {0.0, p.getM() * g_grav, 0.0};
        // p.setF(p.getF() + gravity);

        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));

        p.setF({0, 0, 0});
        // check particle index and potentially move it
        // if the particle somehow goes completely out of bounds, remove it to avoid issues
        int newIdx = lc->getCellIndex(p.getX());
        if (newIdx == -1) {
            p.markInactive();
            p.setCellIndex(-1);
            continue;
        }
        if (newIdx != p.getCellIndex()) {
            SPDLOG_DEBUG("Index mismatch (current: {}, expected: {}), moving...", p.getCellIndex(), newIdx);

            Cell &targetCell = (*lc)[newIdx];
            if (handleHaloCell(p, targetCell, lc))
                continue;

            if (!lc->moveParticle(p))
                p.markInactive();
        }
    }
}