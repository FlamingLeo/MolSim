#include "VelocityCalculation.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateV(ParticleContainer &particles, double delta_t) {
    for (auto &p : particles) {
        const std::array<double, 3> velocityUpdate =
            ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>());
        p.getV() = p.getV() + velocityUpdate;
    }
}