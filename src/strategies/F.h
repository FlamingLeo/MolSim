/**
 * @file F.h
 * @brief Strategy functions for calculating the force effective on one or more particles.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/ParticleContainer.h"

/**
 * @brief Calculates the force \f$ F \f$ for all particles using a naive approach for a Verlet simulation.
 *
 * @details For each particle in the simulation, the method works by first calculating the effective force \f[
 * F_{ij} = \frac{m_i m_j}{(|| x_i - x_j ||_2)^3} (x_j - x_i) \f] between each pair of particles \f$ i \f$ and \f$ j
 * \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[ F_i =
 * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 */
void calculateF_Verlet(ParticleContainer &particles, double, double);

/**
 * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion for a Verlet simulation.
 *
 * @details <em> For every action, there is an equal and opposite reaction.</em>
 *
 * Instead of looping through and calculating the force for each individual particle, we avoid recalculating force
 * for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to both particles
 * simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 */
void calculateF_VerletThirdLaw(ParticleContainer &particles, double, double);

/**
 * @brief TODO
 *
 * @param particles TODO
 * @param epsilon TODO
 * @param sigma TODO
 */
void calculateF_LennardJones(ParticleContainer &particles, double epsilon, double sigma);