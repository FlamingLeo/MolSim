#include "F.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>

void calculateF_Verlet(ParticleContainer &particles, double, double) {
    for (auto &p1 : particles) {
        for (auto &p2 : particles) {
            if (p1 != p2) {
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                        p1.getM() * p2.getM() / std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3),
                                        p2.getX() - p1.getX(), std::multiplies<>()));
            }
        }
    }
}

void calculateF_VerletThirdLaw(ParticleContainer &particles, double, double) {
    // loop over unique pairs
    for (size_t i = 0; i < particles.size(); ++i) {
        auto &p1 = particles[i];

        // avoid recalculating force for pairs which have already been computed
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto &p2 = particles[j];

            auto distVec = p2.getX() - p1.getX();
            double distNorm = ArrayUtils::L2Norm(distVec);

            // prevent division by 0
            if (distNorm == 0)
                continue;

            double forceMag = p1.getM() * p2.getM() / std::pow(distNorm, 3);
            auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());

            // use newton's third law to apply force on p1 and opposite force on p2
            p1.setF(p1.getF() + forceVec);
            p2.setF(p2.getF() - forceVec);
        }
    }
}

void calculateF_LennardJones(ParticleContainer &particles, double epsilon, double sigma) {
    for (auto &p1 : particles) {
        for (auto &p2 : particles) {
            if (p1 != p2) {
                double distNorm = ArrayUtils::L2Norm(p1.getX() - p2.getX());
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                        ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                            (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12)),
                                        p1.getX() - p2.getX(), std::multiplies<>()));
            }
        }
    }
}

void calculateF_LennardJonesThirdLaw(ParticleContainer &particles, double epsilon, double sigma) {
    // loop over unique pairs
    for (size_t i = 0; i < particles.size(); ++i) {
        auto &p1 = particles[i];
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto &p2 = particles[j];

            auto distVec = p1.getX() - p2.getX();
            double distNorm = ArrayUtils::L2Norm(distVec);

            // prevent division by 0
            if (distNorm == 0)
                continue;

            double forceMag = (-24 * epsilon / std::pow(distNorm, 2)) *
                              (std::pow(sigma / distNorm, 6) - 2 * std::pow(sigma / distNorm, 12));
            auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());

            // use newton's third law to apply force on p1 and opposite force on p2
            p1.setF(p1.getF() + forceVec);
            p2.setF(p2.getF() - forceVec);
        }
    }
}