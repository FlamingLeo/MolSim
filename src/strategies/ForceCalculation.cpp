#include "ForceCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateF_Gravity(ParticleContainer &particles, double, double, CellContainer *) {
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

void calculateF_GravityThirdLaw(ParticleContainer &particles, double, double, CellContainer *) {
    // loop over unique pairs
    for (auto pair = particles.beginPairs(); pair != particles.endPairs(); ++pair) {
        auto &p1 = (*pair).first;
        auto &p2 = (*pair).second;

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

void calculateF_LennardJones(ParticleContainer &particles, double epsilon, double sigma, CellContainer *) {
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

void calculateF_LennardJonesThirdLaw(ParticleContainer &particles, double epsilon, double sigma, CellContainer *) {
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

void calculateF_LennardJones_LC(ParticleContainer &particles, double epsilon, double sigma, CellContainer *lc) {
    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all particles i in cell ic
        for (auto *i : ic) {
            if (!i->isActive())
                continue;

            // loop over all cells kc in N(ic)
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];
                // loop over all particles j in cell kc
                for (auto *j : kc) {
                    if (!j->isActive())
                        continue;

                    if (*i != *j) {
                        double distNorm = ArrayUtils::L2Norm(j->getX() - i->getX());
                        if (distNorm <= lc->getCutoff()) {
                            i->setF(i->getF() +
                                    ArrayUtils::elementWiseScalarOp(
                                        ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                            (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12)),
                                        i->getX() - j->getX(), std::multiplies<>()));
                        }
                    }
                }
            }
        }
    }
}

void calculateF_LennardJonesThirdLaw_LC(ParticleContainer &particles, double epsilon, double sigma, CellContainer *lc) {
    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all particles i in cell ic
        for (auto *i : ic) {
            if (!i->isActive())
                continue;

            // loop over all cells kc in N(ic)
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];
                // loop over all particles j in cell kc
                for (auto *j : kc) {
                    if (!j->isActive() || i >= j)
                        continue;

                    // only iterate through distinct pairs by comparing (distinct) memory addresses
                    // in one complete force calculation, either i < j or i >= j will hold, but not both
                    auto distVec = i->getX() - j->getX();
                    double distNorm = ArrayUtils::L2Norm(distVec);

                    if (distNorm <= lc->getCutoff()) {
                        // i could definitely make this look nicer...
                        i->setF(i->getF() +
                                ArrayUtils::elementWiseScalarOp(
                                    ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                        (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12)),
                                    i->getX() - j->getX(), std::multiplies<>()));
                        j->setF(j->getF() -
                                ArrayUtils::elementWiseScalarOp(
                                    ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                        (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12)),
                                    i->getX() - j->getX(), std::multiplies<>()));
                    }
                }
            }
        }
    }
}