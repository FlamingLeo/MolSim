#include "ForceCalculation.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "strategies/BoundaryConditions.h"
#include "utils/ArrayUtils.h"
#include <functional>
#include <spdlog/spdlog.h>

void calculateF_Gravity(ParticleContainer &particles, double, CellContainer *) {
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

void calculateF_GravityThirdLaw(ParticleContainer &particles, double, CellContainer *) {
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
    for (auto &p1 : particles) {
        for (auto &p2 : particles) {
            if (p1 != p2) {
                double epsilon = std::sqrt(p1.getEpsilon() * p2.getEpsilon());
                double sigma = (p1.getSigma() + p2.getSigma()) / 2;
                double distNorm = ArrayUtils::L2Norm(p1.getX() - p2.getX());
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                        ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                            (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12)),
                                        p1.getX() - p2.getX(), std::multiplies<>()));
            }
        }
    }
}

void calculateF_LennardJonesCutoff(ParticleContainer &particles, double cutoff, CellContainer *) {
    for (auto &p1 : particles) {
        for (auto &p2 : particles) {
            if (p1 != p2) {
                double epsilon = std::sqrt(p1.getEpsilon() * p2.getEpsilon());
                double sigma = (p1.getSigma() + p2.getSigma()) / 2;
                double distNorm = ArrayUtils::L2Norm(p1.getX() - p2.getX());
                if (distNorm <= cutoff) {
                    p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(((-24 * epsilon) / std::pow(distNorm, 2)) *
                                                                            (std::pow((sigma / distNorm), 6) -
                                                                             2 * std::pow((sigma / distNorm), 12)),
                                                                        p1.getX() - p2.getX(), std::multiplies<>()));
                }
            }
        }
    }
}

void calculateF_LennardJonesThirdLaw(ParticleContainer &particles, double, CellContainer *) {
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

void calculateF_LennardJonesThirdLawCutoff(ParticleContainer &particles, double cutoff, CellContainer *) {
    // loop over unique pairs
    for (size_t i = 0; i < particles.size(); ++i) {
        auto &p1 = particles[i];
        for (size_t j = i + 1; j < particles.size(); ++j) {
            auto &p2 = particles[j];

            auto distVec = p1.getX() - p2.getX();
            double distNorm = ArrayUtils::L2Norm(distVec);

            if (distNorm <= cutoff) {
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
}

void calculateF_LennardJones_LC(ParticleContainer &particles, double, CellContainer *lc) {
    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all particles i in cell ic
        for (auto &ri : ic) {
            Particle &i = ri;
            if (!i.isActive())
                continue;

            // loop over all cells kc in N(ic)
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];
                // loop over all particles j in cell kc
                for (auto &rj : kc) {
                    Particle &j = rj;
                    if (!j.isActive())
                        continue;

                    if (i != j) {
                        auto distVec = i.getX() - j.getX();
                        double distNorm = ArrayUtils::L2Norm(distVec);

                        // compute force if distance is less than cutoff
                        if (distNorm <= lc->getCutoff()) {
                            double epsilon = std::sqrt(i.getEpsilon() * j.getEpsilon());
                            double sigma = (i.getSigma() + j.getSigma()) / 2;

                            double forceMag = ((-24 * epsilon) / std::pow(distNorm, 2)) *
                                              (std::pow((sigma / distNorm), 6) - 2 * std::pow((sigma / distNorm), 12));
                            auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());
                            i.setF(i.getF() + forceVec);
                        }
                    }
                }
            }
        }
    }
}

void calculateF_LennardJonesThirdLaw_LC(ParticleContainer &particles, double, CellContainer *lc) {
    SPDLOG_DEBUG("Start F");
    //FIRST WE MIRROR BORDER PARTICLES IF WE HAVE PERIODIC BOUNDARY CONDITIONS
    if(std::find(lc->getConditions().begin(), lc->getConditions().end(), BoundaryCondition::PERIODIC) != lc->getConditions().end()) {
        mirrorGhostParticles(lc);
    }

    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all active particles i in cell ic

        //EXTRA SPECIAL CASE: if halo cell skip (no multiple interactions between same particles)
        if(!ic.getHaloLocation().empty()){
            continue;
        }

        //for every particle
        for (auto &ri : ic) {
            Particle &i = ri;
            if (!i.isActive())
                continue;

            // loop over all cells kc in Neighbours(ic)
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];

                for (auto &rj : kc) {

                    //normal check
                    Particle &j = rj;
                    if (!j.isActive() || &i >= &j)
                        continue;

                    std::array<double, 3> truePos = j.getX();
                    //SPECIAL CASE: Particle j is a ghost particle
                    if(!kc.getHaloLocation().empty()) {
                        //We need to fake the position of the ghost Particle as if it were in the halo cell
                        Cell trueCell = lc->getCells()[j.getCellIndex()];
                        std::array<double, 3> inCell = {j.getX()[0] - trueCell.getX()[0],
                                                        j.getX()[1] - trueCell.getX()[1],
                                                        j.getX()[2] - trueCell.getX()[2]};
                        truePos = {kc.getX()[0] + inCell[0], kc.getX()[1] + inCell[1],
                                   kc.getX()[2] + inCell[2]};

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
                        i.setF(i.getF() + forceVec);
                        j.setF(j.getF() - forceVec);
                    }
                }
            }
        }
    }
    //IN THE END WE DELETE GHOST PARTICLES IF WE HAVE PERIODIC CONDITIONS
    if(std::find(lc->getConditions().begin(), lc->getConditions().end(), BoundaryCondition::PERIODIC) != lc->getConditions().end()) {
        deleteGhostParticles(lc);
    }
}

void calculateF_LennardJonesThirdLaw_LC_Periodic(ParticleContainer &particles, double, CellContainer *lc) {

    //FIRST WE MIRROR BORDER PARTICLES IF WE HAVE PERIODIC BOUNDARY CONDITIONS
    if(std::find(lc->getConditions().begin(), lc->getConditions().end(), BoundaryCondition::PERIODIC) != lc->getConditions().end()) {
        mirrorGhostParticles(lc);
    }

    // loop over all cells ic
    for (auto &ic : *lc) {
        // loop over all active particles i in cell ic

        //EXTRA SPECIAL CASE: if halo cell skip (no multiple interactions between same particles)
        if(!ic.getHaloLocation().empty()){
            continue;
        }

        //for every particle
        for (auto &ri : ic) {
            Particle &i = ri;
            if (!i.isActive())
                continue;

            // loop over all cells kc in Neighbours(ic)
            for (size_t kci : lc->getNeighbors(ic.getIndex())) {
                Cell &kc = (*lc)[kci];

                for (auto &rj : kc) {

                    //normal check
                    Particle &j = rj;
                    if (!j.isActive() || &i >= &j)
                        continue;

                    std::array<double, 3> truePos = j.getX();
                    //SPECIAL CASE: Particle j is a ghost particle
                    if(!kc.getHaloLocation().empty()) {
                        //We need to fake the position of the ghost Particle as if it were in the halo cell
                        Cell trueCell = lc->getCells()[j.getCellIndex()];
                        std::array<double, 3> inCell = {j.getX()[0] - trueCell.getX()[0],
                                                        j.getX()[1] - trueCell.getX()[1],
                                                        j.getX()[2] - trueCell.getX()[2]};
                        truePos = {kc.getX()[0] + inCell[0], kc.getX()[1] + inCell[1],
                                                         kc.getX()[2] + inCell[2]};

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
                        i.setF(i.getF() + forceVec);
                        j.setF(j.getF() - forceVec);
                    }
                }
            }
        }
    }
    //IN THE END WE DELETE GHOST PARTICLES IF WE HAVE PERIODIC CONDITIONS
    if(std::find(lc->getConditions().begin(), lc->getConditions().end(), BoundaryCondition::PERIODIC) != lc->getConditions().end()) {
        deleteGhostParticles(lc);
    }
}

void addGravitationalForce(ParticleContainer &particles, double g_grav) {
    // go over all particles and add gravity along the y-axis
    for (auto &p : particles) {
        if (!p.isActive()) {
            continue;
        }

        std::array<double, 3> gravity = {0.0, p.getM() * g_grav, 0.0};
        p.setF(p.getF() + gravity);
    }
}