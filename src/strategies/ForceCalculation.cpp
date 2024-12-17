#include "ForceCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "strategies/BoundaryConditions.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

// helper function to calculate force vector
std::array<double, 3> getForceVec(const Particle &p1, const Particle &p2, const std::array<double, 3> distVec,
                                  double distNorm) {
    double epsilon = std::sqrt(p1.getEpsilon() * p2.getEpsilon());
    double sigma = (p1.getSigma() + p2.getSigma()) / 2;

    // precompute terms
    // pow(x, 2) _may_ allegedly be faster...
    double distNormSquared = std::pow(distNorm, 2);
    double invDistNorm = 1.0 / distNorm;
    double sigmaOverDist = sigma * invDistNorm;
    double sigmaOverDist2 = sigmaOverDist * sigmaOverDist;
    double sigmaOverDist6 = sigmaOverDist2 * sigmaOverDist2 * sigmaOverDist2; // (sigma / dist) ^ 6
    double sigmaOverDist12 = sigmaOverDist6 * sigmaOverDist6;                 // (sigma / dist) ^ 12

    // compute force magnitude
    double forceMag = ((-24 * epsilon) / distNormSquared) * (sigmaOverDist6 - 2 * sigmaOverDist12);

    // return force vector
    return ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());
}

// helper method to potentially fake the position of a ghost particle, for use with periodic boundaries
std::array<double, 3> getTruePos(const Particle &p, const Cell &c, CellContainer *lc) {
    // special case: particle j is a ghost particle
    // otherwise, we just get the real position of the particle
    if (!c.getHaloLocation().empty()) {
        // we need to fake the position of the ghost particle as if it were in the halo cell
        Cell &trueCell = lc->getCells()[p.getCellIndex()];
        std::array<double, 3> inCell = {p.getX()[0] - trueCell.getX()[0], p.getX()[1] - trueCell.getX()[1],
                                        p.getX()[2] - trueCell.getX()[2]};
        return {c.getX()[0] + inCell[0], c.getX()[1] + inCell[1], c.getX()[2] + inCell[2]};
    } else {
        return p.getX();
    }
}

/* documented functions start here */
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

            // calculate force
            auto forceVec = getForceVec(p1, p2, distVec, distNorm);

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
        // special case: if halo cell, skip (no multiple interactions between same particles)
        if (!ic.getHaloLocation().empty()) {
            continue;
        }

        // loop over all active particles i in cell ic
        for (auto &ri : ic) {
            Particle &i = ri;
            if (!i.isActive())
                continue;

            // loop over all cells kc in Neighbours(ic), including the particle i's own cell
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];

                // loop over all particles j in kc
                for (auto &rj : kc) {
                    // check if j is active AND if i and j form a distinct pair (N3L)
                    // for checking distinct pairs, we compare the memory addresses of the two particles
                    Particle &j = rj;
                    if (!j.isActive() || &i >= &j)
                        continue;

                    // get the position used to calculate the distance between to particles
                    std::array<double, 3> truePos = getTruePos(j, kc, lc);

                    // calculate the distance between the two particles
                    auto distVec = i.getX() - truePos;
                    double distNorm = ArrayUtils::L2Norm(distVec);

                    // compute force if distance is less than cutoff
                    if (distNorm <= lc->getCutoff()) {
                        // calculate force
                        auto forceVec = getForceVec(i, j, distVec, distNorm);

                        // apply force on particle i (no force on ghost particle)
                        i.getF() = i.getF() + forceVec;
                        j.getF() = j.getF() - forceVec;
                    }
                }
            }
        }
    }

    // delete ghost particles in the end
    if (VEC_CONTAINS(lc->getConditions(), BoundaryCondition::PERIODIC))
        deleteGhostParticles(lc);
}