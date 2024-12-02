/**
 * @file ForceCalculation.h
 * @brief Strategy functions for calculating the force effective on one or more particles.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a Gravity simulation.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = \frac{m_i m_j}{(|| x_i - x_j ||_2)^3} (x_j - x_i) \f] between each pair of particles \f$ i \f$ and \f$ j
 * \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_Gravity(ParticleContainer &particles, double, double, double, CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion for a Gravity simulation.
 *
 * @details <em> For every action, there is an equal and opposite reaction.</em>
 *
 * Instead of looping through and calculating the force for each individual particle, we avoid recalculating force
 * for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to both particles
 * simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_GravityThirdLaw(ParticleContainer &particles, double, double, double, CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a LennardJones simulation.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = -\frac{24 \cdot \epsilon}{\left(\|x_i - x_j\|_2\right)^2} \left( \left( \frac{\sigma}{\|x_i - x_j\|_2}
 \right)^6 - 2 \left( \frac{\sigma}{\|x_i - x_j\|_2} \right)^{12} \right) (x_i - x_j) \f] between each pair of particles
 \f$ i \f$ and \f$ j
 * \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_LennardJones(ParticleContainer &particles, double epsilon, double sigma, double,
                             CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a LennardJones simulation and a
 specified cutoff radius.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = -\frac{24 \cdot \epsilon}{\left(\|x_i - x_j\|_2\right)^2} \left( \left( \frac{\sigma}{\|x_i - x_j\|_2}
 \right)^6 - 2 \left( \frac{\sigma}{\|x_i - x_j\|_2} \right)^{12} \right) (x_i - x_j) \f] between each pair of particles
 \f$ i \f$ and \f$ j
 * \f$ ONLY if the distance between them is less than the cutoff radius. Afterwards, the effective force for a single
 particle \f$ i \f$ is calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param cutoff The cutoff radius.
 */
void calculateF_LennardJonesCutoff(ParticleContainer &particles, double epsilon, double sigma, double cutoff,
                                   CellContainer *);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion for a LennardJones
 * simulation.
 *
 * @details <em> For every action, there is an equal and opposite reaction.</em>
 *
 * Instead of looping through and calculating the force for each individual particle, we avoid recalculating force
 * for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to both particles
 * simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_LennardJonesThirdLaw(ParticleContainer &particles, double epsilon, double sigma, double,
                                     CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion for a LennardJones
 * simulation and a cutoff radius.
 *
 * @details <em> For every action, there is an equal and opposite reaction.</em>
 *
 * Instead of looping through and calculating the force for each individual particle, we avoid recalculating force
 * for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to both particles
 * simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
 *
 * We only take particles that have a distance smaller than (or equal to) the cutoff radius to eachother into
 * consideration.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param cutoff The cutoff radius.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_LennardJonesThirdLawCutoff(ParticleContainer &particles, double epsilon, double sigma, double cutoff,
                                           CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a linked-cell LennardJones
 simulation and a cutoff radius.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = -\frac{24 \cdot \epsilon}{\left(\|x_i - x_j\|_2\right)^2} \left( \left( \frac{\sigma}{\|x_i - x_j\|_2}
 \right)^6 - 2 \left( \frac{\sigma}{\|x_i - x_j\|_2} \right)^{12} \right) (x_i - x_j) \f] between each pair of particles
 \f$ i \f$ and \f$ j
 * \f$ in the neighboring (and current) cells. Afterwards, the effective force for a single particle \f$ i \f$ is
 calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles within the cell's region.
 *
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateF_LennardJones_LC(ParticleContainer &particles, double epsilon, double sigma, double, CellContainer *lc);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion for a linked-cell
 * LennardJones simulation and a cutoff radius.
 *
 * @details <em> For every action, there is an equal and opposite reaction.</em>
 *
 * Instead of looping through and calculating the force for each individual particle in the cell neighborhood, we avoid
 * recalculating force for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to
 * both particles simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
 *
 * We only take particles that have a distance smaller than (or equal to) the cutoff radius to eachother into
 * consideration.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateF_LennardJonesThirdLaw_LC(ParticleContainer &particles, double epsilon, double sigma, double,
                                        CellContainer *lc);