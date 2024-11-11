#include "V.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>

void calculateV(ParticleContainer &particles, double delta_t) {
    for (auto &p : particles) {
        p.setV(p.getV() +
               ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>()));
    }
}