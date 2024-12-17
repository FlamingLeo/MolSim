#include "ForceCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "strategies/BoundaryConditions.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateF_Gravity(ParticleContainer &particles, double, CellContainer *) {
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

void calculateF_LennardJones(ParticleContainer &particles, double, CellContainer *) {
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

            double epsilon = std::sqrt(p1.getEpsilon() * p2.getEpsilon());
            double sigma = (p1.getSigma() + p2.getSigma()) / 2;

            double forceMag = (-24 * epsilon / std::pow(distNorm, 2)) *
                              (std::pow(sigma / distNorm, 6) - 2 * std::pow(sigma / distNorm, 12));
            auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());

            // use newton's third law to apply force on p1 and opposite force on p2
            p1.setF(p1.getF() + forceVec);
            p2.setF(p2.getF() - forceVec);
        }
    }
}

void calculateF_LennardJones_LC(ParticleContainer &particles, double, CellContainer *lc) {
    // mirror border particles for periodic boundaries
    if (VEC_CONTAINS(lc->getConditions(), BoundaryCondition::PERIODIC))
        mirrorGhostParticles(lc);

    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all active particles i in cell ic
        // special case: if halo cell, skip (no multiple interactions between same particles)
        if (!ic.getHaloLocation().empty()) {
            continue;
        }

        // for every particle i
        for (auto &ri : ic) {
            Particle &i = ri;
            if (!i.isActive())
                continue;

            // loop over all cells kc in Neighbours(ic), including the particle i's own cell
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];

                // for every particle j in kc
                for (auto &rj : kc) {
                    // check if j is active AND if i and j form a distinct pair (N3L)
                    // for checking distinct pairs, we compare the memory addresses of the two particles
                    Particle &j = rj;
                    if (!j.isActive() || &i >= &j)
                        continue;

                    std::array<double, 3> truePos = j.getX();
                    // special case: particle j is a ghost particle
                    if (!kc.getHaloLocation().empty()) {
                        // we need to fake the position of the ghost particle as if it were in the halo cell
                        Cell &trueCell = lc->getCells()[j.getCellIndex()];
                        std::array<double, 3> inCell = {j.getX()[0] - trueCell.getX()[0],
                                                        j.getX()[1] - trueCell.getX()[1],
                                                        j.getX()[2] - trueCell.getX()[2]};
                        truePos = {kc.getX()[0] + inCell[0], kc.getX()[1] + inCell[1], kc.getX()[2] + inCell[2]};
                    }

                    auto distVec = i.getX() - truePos;
                    double distNorm = ArrayUtils::L2Norm(distVec);

                    // compute force if distance is less than cutoff
                    if (distNorm <= lc->getCutoff()) {
                        double epsilon = std::sqrt(i.getEpsilon() * j.getEpsilon());
                        double sigma = (i.getSigma() + j.getSigma()) / 2;

                        double forceMag = ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                          (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12));
                        auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());

                        // apply force on particle i (no force on ghost particle)
                        i.getF() = i.getF() + forceVec;
                        j.getF() = j.getF() - forceVec;
                    }
                }
            }
        }
    }

    // delete ghost particles in the end
    if (std::find(lc->getConditions().begin(), lc->getConditions().end(), BoundaryCondition::PERIODIC) !=
        lc->getConditions().end()) {
        deleteGhostParticles(lc);
    }
}