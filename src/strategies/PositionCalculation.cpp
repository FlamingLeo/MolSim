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
        // update position
        const std::array<double, 3> posSum1 = ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>());
        const std::array<double, 3> posSum2 =
            ArrayUtils::elementWiseScalarOp((delta_t * delta_t) / (2 * p.getM()), p.getF(), std::multiplies<>());
        p.getX() = p.getX() + posSum1 + posSum2;

        // store previous force, then reset force to 0
        p.setOldF(p.getF());
        p.setF({0.0, p.getM() * g_grav, 0.0});
    }
}

void calculateX_LC(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *lc) {
    SPDLOG_TRACE("Calculating new position (linked cells)...");
    for (auto &p : particles) {
        if (!p.isActive())
            continue;

        // update position (maybe precompute dt^2, even though it's probably only marginally faster, if anything)
        const std::array<double, 3> posSum1 = ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>());
        const std::array<double, 3> posSum2 =
            ArrayUtils::elementWiseScalarOp((delta_t * delta_t) / (2 * p.getM()), p.getF(), std::multiplies<>());
        p.getX() = p.getX() + posSum1 + posSum2;

        // store previous force for velocity calculation, then reset force to 0
        // optimization: add graviational force here
        // we can do this because the forces are additive; it doesn't matter if we first calculate the forces between
        // the particles or the gravitational force
        // thus, we save having to iterate through all particles once again after calculating the force
        p.getOldF() = p.getF();
        p.setF({0.0, p.getM() * g_grav, 0.0});

        // check to see if the particle's cell index got updated
        int newIdx = lc->getCellIndex(p.getX());

        // if the particle is somehow out of bounds, remove it
        // note: this could probably be moved inside the next if statement...
        if (newIdx == -1) {
            SPDLOG_ERROR("Particle {} out of bounds! Removing...", p.toString());
            p.markInactive();
            p.setCellIndex(-1);
            continue;
        }

        // if the particle's stored cell index does not match the newly calculated index, move it
        if (newIdx != p.getCellIndex()) {
            SPDLOG_TRACE("Index mismatch (current: {}, expected: {}), moving...", p.getCellIndex(), newIdx);
            Cell &targetCell = (*lc)[newIdx];

            // check if the particle entered a halo cell and apply the correct boundary condition
            if (handleHaloCell(p, targetCell, lc))
                continue;

            // move particle (update stored cell index)
            if (!lc->moveParticle(p)) {
                SPDLOG_ERROR("Cannot move particle {}!", p.toString());
                p.markInactive();
            }
        }
    }
}