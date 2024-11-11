#include "X.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>

void calculateX(ParticleContainer &particles, double delta_t) {
    for (auto &p : particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
    }
}