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
 * **This is the slowest of all LennardJones force calculation functions.**
 *
 * **Complexity:** \f$ O(N^2) \f$
 *
 * The figure below shows a comparison of all LennardJones force calculation functions using a 2D square as input (as
 per Worksheet 3) with the elapsed time in ms on the y-axis and the size of the square's side on the x-axis.
 *
 * @image html lj-complete.png A comparison of all LJ force calculation functions.
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
 * @details For each particle in the simulation, the method works by first calculating the effective force between each
 pair of particles \f$ i \f$ and \f$ j \f$ ONLY if the distance between them is less than the cutoff radius. The rest is
 analogous to the default naive function.
 *
 * **Complexity:** \f$ O(N^2) \f$
 *
 * The figure below shows a comparison between the force calculation mehods which use a cutoff radius and the linked
 cell variants. The input and the axis descriptions are the same as in the naive algorithm graph.
 *
 * @image html lj-cutoff.png A comparison of the cutoff LJ force calculation methods alongside the linked cell variants.
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
 * **Complexity:** \f$ O(N^2) \f$
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
 * @details Newton's Third Law is used, but now, the algorithm only takes particles that have a distance smaller than
 * (or equal to) the cutoff radius to eachother into consideration.
 *
 * **Complexity:** \f$ O(N^2) \f$
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
 * @details This is an adaptation of the naive Lennard-Jones force calculation, but instead of iterating through every
 * particle pair in the entire container, the algorithm only uses particles within the cell neighborhood of the current
 * particle.
 *
 * **Complexity:** \f$ O(N) \f$
 *
 * The figure below shows the comparison of using the linked cell method with and without applying Newton's third law.
 The input and the axis descriptions are the same as in the naive algorithm graph.
 *
 * @image html lj-lc.png A comparison of the linked-cell LJ methods.
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
 * @details Newton's Third Law is used, but now, the algorithm only takes particles within a particle's cell
 * neighborhood that have a distance smaller than (or equal to) the cutoff radius to eachother into consideration.
 *
 * **Complexity:** \f$ O(N) \f$
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param epsilon The depth of the potential well \f$ \epsilon \f$
 * @param sigma The distance \f$ \sigma \f$ at which the particle-particle potential energy is zero.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateF_LennardJonesThirdLaw_LC(ParticleContainer &particles, double epsilon, double sigma, double,
                                        CellContainer *lc);