#include "VelocityCalculation.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateV(ParticleContainer &particles, double delta_t) {
    for (auto &p : particles) {
        if (!p.isActive())
            continue;

        p.setV(p.getV() +
               ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>()));
    }
}