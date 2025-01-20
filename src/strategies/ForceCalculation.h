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
#define SQR(x) ((x) * (x))

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a Gravity simulation.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = \frac{m_i m_j}{(|| x_i - x_j ||_2)^3} (x_j - x_i) \f] between each **distinct** pair of particles \f$ i \f$
 * and \f$ j \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[
 * F_i = \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * This method uses Newton's Third Law \f[ F_{ij} = -F_{ji}. \f] to avoid calculating the force twice.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_Gravity(ParticleContainer &particles, double, CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a LennardJones simulation.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = -\frac{24 \cdot \epsilon}{\left(\|x_i - x_j\|_2\right)^2} \left( \left( \frac{\sigma}{\|x_i - x_j\|_2}
 \right)^6 - 2 \left( \frac{\sigma}{\|x_i - x_j\|_2} \right)^{12} \right) (x_i - x_j) \f] between each **distinct** pair
 of particles \f$ i \f$ and \f$ j
 * \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * This method uses Newton's Third Law \f[ F_{ij} = -F_{ji}. \f] to avoid calculating the force twice.
 *
 * **Complexity:** \f$ O(N^2) \f$
 *
 * The figure below shows a comparison of all previously implemented LennardJones force calculation functions using a 2D
 square as input (as per Worksheet 3) with the elapsed time in ms on the y-axis and the number of particles the x-axis.
 Note that many have been removed to streamline optimization and testing.
 *
 * @image html lj-complete.png A comparison of all LJ force calculation functions.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateF_LennardJones(ParticleContainer &particles, double, CellContainer *lc = nullptr);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a linked-cell LennardJones
 simulation and a cutoff radius.
 *
 * @details This is an adaptation of the naive Lennard-Jones force calculation, but instead of iterating through every
 * particle pair in the entire container, the algorithm only uses particles within the cell neighborhood of the current
 * particle.
 *
 * For parallelization, the domain is split into chunks distributed amonst threads. This is done using standard OpenMP
 * loop parallelization.
 *
 * **Complexity:** \f$ O(N) \f$
 *
 * This method uses Newton's Third Law \f[ F_{ij} = -F_{ji}. \f] to avoid calculating the force twice.
 *
 * The figure below shows the comparison of using the linked cell method with and without applying Newton's third law.
 The input and the axis descriptions are the same as in the naive algorithm graph.
 *
 * @image html lj-lc.png A comparison of the linked-cell LJ methods.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateF_LennardJones_LC(ParticleContainer &particles, double, CellContainer *lc);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a linked-cell LennardJones
 simulation and a cutoff radius using a task-based parallelization approach.
 *
 * @details Fundamentally, this is the same algorithm as the base aslgorithm, but instead of splitting chunks of the
 * domain amongst a team of threads, each cell is assigned a separate OpenMP task, distributed among threads via a
 * single delegation thread.
 *
 * **Complexity:** \f$ O(N) \f$
 *
 * This method uses Newton's Third Law \f[ F_{ij} = -F_{ji}. \f] to avoid calculating the force twice.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateF_LennardJones_LC_task(ParticleContainer &particles, double, CellContainer *lc);