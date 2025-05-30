#include "ForceCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "strategies/BoundaryConditions.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <vector>

// helper function to calculate force vector
static inline std::array<double, 3> getLJForceVec(const Particle &p1, const Particle &p2,
                                                  const std::array<double, 3> distVec, double distNorm) {
    double epsilon = std::sqrt(p1.getEpsilon() * p2.getEpsilon());
    double sigma = (p1.getSigma() + p2.getSigma()) / 2;

    // precompute terms
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

// helper function to check whether particles are neighbours
static inline bool inNeighbourVector(const std::vector<std::reference_wrapper<Particle>> vec,
                                     const std::reference_wrapper<Particle> el) {
    for (auto &vec_el : vec) {
        if (vec_el.get() == el.get()) {
            return true;
        }
    }
    return false;
}

// helper method to potentially fake the position of a ghost particle, for use with periodic boundaries
static inline std::array<double, 3> getTruePos(const Particle &p, const Cell &c, CellContainer *lc) {
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
            auto forceVec = getLJForceVec(p1, p2, distVec, distNorm);

            // use newton's third law to apply force on p1 and opposite force on p2
            DO_IF_NOT_WALL(p1, p1.setF(p1.getF() + forceVec));
            DO_IF_NOT_WALL(p2, p2.setF(p2.getF() - forceVec));
        }
    }
}

void calculateF_LennardJones_LC(ParticleContainer &particles, double, CellContainer *lc) {
    // mirror border particles for periodic boundaries
    if (lc->getAnyPeriodic())
        mirrorGhostParticles(lc);

// loop over all (regular) cells ic
#pragma omp parallel for
    CONTAINER_LOOP(lc->getIterableCells(), it) {
        // loop over all active particles i in cell ic
        auto &ic = CONTAINER_REF(it);
        for (auto &ri : ic.get()) {
            // loop over all cells kc in Neighbours(ic), including the particle i's own cell
            for (auto &kc : lc->getNeighborCells(ic.get().getIndex())) {
                // loop over all particles j in kc
                for (auto &rj : kc.get()) {
                    // check if i and j form a distinct pair (N3L)
                    // note: we don't need to check for activity here, since all particles are guaranteed active
                    //       all inactive particles have since been removed from the cells
                    // for checking distinct pairs, we compare the memory addresses of the two particles
                    Particle &i = ri;
                    Particle &j = rj;
                    if (&i >= &j)
                        continue;

                    // get the position used to calculate the distance between to particles
                    std::array<double, 3> truePos = getTruePos(j, kc.get(), lc);

                    // calculate the distance between the two particles
                    auto distVec = i.getX() - truePos;
                    double dist = ArrayUtils::L2NormSquared(distVec);

                    // compute force if distance is less than cutoff
                    if (dist <= SQR(lc->getCutoff())) {
                        // calculate force
                        double distNorm = std::sqrt(dist);
                        auto forceVec = getLJForceVec(i, j, distVec, distNorm);

                        // apply force on non-wall particles (no force on ghost particle)
                        omp_set_lock(&i.getLock());
                        DO_IF_NOT_WALL(i, i.getF() = i.getF() + forceVec);
                        omp_unset_lock(&i.getLock());

                        omp_set_lock(&j.getLock());
                        DO_IF_NOT_WALL(j, j.getF() = j.getF() - forceVec);
                        omp_unset_lock(&j.getLock());
                    }
                }
            }
        }
    }

    // delete ghost particles in the end
    if (lc->getAnyPeriodic())
        deleteGhostParticles(lc);
}

void calculateF_LennardJones_LC_task(ParticleContainer &particles, double, CellContainer *lc) {
    // mirror border particles for periodic boundaries
    if (lc->getAnyPeriodic())
        mirrorGhostParticles(lc);

#pragma omp parallel
    {
#pragma omp single nowait
        {
            // loop over all (regular) cells ic
            for (auto &ic : lc->getIterableCells()) {
#pragma omp task firstprivate(ic)
                {
                    // loop over all active particles i in cell ic
                    for (auto &ri : ic.get()) {
                        // loop over all cells kc in Neighbours(ic), including the particle i's own cell
                        for (auto &kc : lc->getNeighborCells(ic.get().getIndex())) {
                            // loop over all particles j in kc
                            for (auto &rj : kc.get()) {
                                // check if i and j form a distinct pair (N3L)
                                Particle &i = ri;
                                Particle &j = rj;
                                if (&i >= &j)
                                    continue;

                                // get the position used to calculate the distance between to particles
                                std::array<double, 3> truePos = getTruePos(j, kc.get(), lc);

                                // calculate the distance between the two particles
                                auto distVec = i.getX() - truePos;
                                double dist = ArrayUtils::L2NormSquared(distVec);

                                // compute force if distance is less than cutoff
                                if (dist <= SQR(lc->getCutoff())) {
                                    // calculate force
                                    double distNorm = std::sqrt(dist);
                                    auto forceVec = getLJForceVec(i, j, distVec, distNorm);

                                    // apply force on particle i (no force on ghost particle)
                                    // technically, we don't even need to check if the particle is a wall, because we
                                    // don't use its force for anything (as in, we skip updating the positions and
                                    // velocities of such particles), so these checks could be omitted in the name of
                                    // speed... but the contest is already done, so check that commit instead :)
                                    omp_set_lock(&i.getLock());
                                    DO_IF_NOT_WALL(i, i.getF() = i.getF() + forceVec);
                                    omp_unset_lock(&i.getLock());

                                    omp_set_lock(&j.getLock());
                                    DO_IF_NOT_WALL(j, j.getF() = j.getF() - forceVec);
                                    omp_unset_lock(&j.getLock());
                                }
                            }
                        }
                    }
                } // end task
            }
        } // end single region
    } // end parallel region

    // delete ghost particles in the end
    if (lc->getAnyPeriodic())
        deleteGhostParticles(lc);
}

void calculateF_Membrane_LC(ParticleContainer &particles, double, CellContainer *lc) {
    SPDLOG_TRACE("r0: {}, k: {}", particles[0].getR0(), particles[0].getK());

    // mirror border particles for periodic boundaries
    if (VEC_CONTAINS(lc->getConditions(), BoundaryCondition::PERIODIC))
        mirrorGhostParticles(lc);

// loop over all cells ic
#pragma omp parallel for
    CONTAINER_LOOP(lc->getIterableCells(), it) {
        auto &ic = CONTAINER_REF(it);
        // loop over all active particles i in cell ic
        for (auto &ri : ic.get()) {
            Particle &i = ri;
            // add special force to the particles that are concerned
            // add a gravitational force on the z-axis (NOT ON THE Y AXIS AS PER USUAL)
            // again, specific to the given scenario
            if (particles.getSpecialForceLimit() != 0) {
                omp_set_lock(&i.getLock());
                i.getF()[2] += i.getFZUP();
                omp_unset_lock(&i.getLock());
                SPDLOG_TRACE("Added force (gravity + F_Z-UP): {}", ArrayUtils::to_string(i.getF()));
            }

            // loop over all cells kc in Neighbours(ic), including the particle i's own cell
            for (auto &kc : lc->getNeighborCells(ic.get().getIndex())) {
                // loop over all particles j in kc
                for (auto &rj : kc.get()) {
                    // check if j is active AND if i and j form a distinct pair (N3L)
                    // for checking distinct pairs, we compare the memory addresses of the two particles
                    Particle &j = rj;
                    if (&i >= &j)
                        continue;

                    std::array<double, 3> truePos = getTruePos(j, kc.get(), lc);
                    std::array<double, 3> forceVec = {0.0, 0.0, 0.0};

                    // calculate the distance between the two particles
                    auto distVec = i.getX() - truePos;
                    double distNorm = ArrayUtils::L2Norm(distVec);
                    double specialCutoff = LJTHRESHOLD * ((i.getSigma() + j.getSigma()) / 2.0);

                    if (inNeighbourVector(i.getDirectNeighbours(), rj)) {
                        // compute scalar for direct neighbors
                        double scalar = i.getK() * (distNorm - i.getR0()) * (1 / distNorm);
                        // because as a distance we use x_i - x_j as distVec when the formula says x_j - x_i,
                        // we multiply by -1
                        forceVec = ArrayUtils::elementWiseScalarOp(-scalar, distVec, std::multiplies<>());
                    } else if (inNeighbourVector(i.getDiagonalNeighbours(), rj)) {
                        // compute scalar for diagonal neighbors
                        double scalar = i.getK() * (distNorm - SQRT2 * i.getR0()) * (1 / distNorm);
                        forceVec = ArrayUtils::elementWiseScalarOp(-scalar, distVec, std::multiplies<>());
                    } else if (distNorm <= specialCutoff) {
                        // calculate special LJ force to prevent self-pen
                        forceVec = getLJForceVec(i, j, distVec, distNorm);
                    }

                    // apply force on particle i (no force on ghost particle)
                    omp_set_lock(&i.getLock());
                    i.getF() = i.getF() + forceVec;
                    omp_unset_lock(&i.getLock());

                    omp_set_lock(&j.getLock());
                    j.getF() = j.getF() - forceVec;
                    omp_unset_lock(&j.getLock());
                }
            }
        }
    }

    // decrement special case iteration count
    if (particles.getSpecialForceLimit() != 0) {
        particles.decrementSpecialForceLimit();
    }

    // delete ghost particles in the end
    if (VEC_CONTAINS(lc->getConditions(), BoundaryCondition::PERIODIC))
        deleteGhostParticles(lc);
}