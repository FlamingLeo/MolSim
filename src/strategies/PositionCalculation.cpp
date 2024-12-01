#include "PositionCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

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

        // check particle index and potentially move it
        // TODO boundary conditions?
        int newIdx = lc->getCellIndex(p.getX());
        if (newIdx != p.getCellIndex()) {
            SPDLOG_DEBUG("Index mismatch (current: {}, expected: {}), moving...", p.getCellIndex(), newIdx);
            if (!lc->moveParticle(p))
                p.markInactive();
        }
    }
}