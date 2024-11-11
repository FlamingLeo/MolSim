/**
 * @file V.h
 * @brief Strategy functions for calculating the velocity of one or more particles.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/ParticleContainer.h"

/**
 * @brief Calculates the velocity \f$ v \f$ for all Particle objects in a given ParticleContainer.
 *
 * @details The velocity \f$ v_i \f$ of a given particle \f$ i \f$ at the next unit of time \f$ t_{n+1} \f$ is
 * calculated using the formula \f[ v_i(t_{n+1}) = v_i(t_n) + \Delta t \frac{F_i(t_n) + F_i(t_{n+1})}{2m_i}. \f]
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param delta_t The timestep \f$ \Delta t \f$.
 */
void calculateV(ParticleContainer &particles, double delta_t);