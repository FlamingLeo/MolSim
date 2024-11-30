#include "PositionCalculation.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>

void calculateX(ParticleContainer &particles, double delta_t) {
    for (auto &p : particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));

        // set previous f for each particle and reinitialize to zero for the upcoming force calculation
        p.setOldF(p.getF());
        p.setFToZero();
    }
}