#include "VelocityCalculation.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/OMPWrapper.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateV(ParticleContainer &particles, double delta_t) {
#pragma omp parallel for
    CONTAINER_LOOP(particles, it) {
        auto &p = CONTAINER_REF(it);
        CONTINUE_IF_INACTIVE(p);

        // skip wall particles
        if (p.getType() != 0)
            continue;

        // calculate velocity
        const std::array<double, 3> velocityUpdate =
            ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>());
        p.getV() = p.getV() + velocityUpdate;
    }
}