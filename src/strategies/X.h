/**
 * @file X.h
 * @brief Strategy functions for calculating the position of one or more particles.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/ParticleContainer.h"

/**
 * @brief Calculates the position \f$ x \f$ for all Particle objects in a given ParticleContainer.
 *
 * @details The position \f$ x_i \f$ of a given particle \f$ i \f$ at the next unit of time \f$ t_{n+1} \f$ is
 * calculated using the formula \f[ x_i(t_{n+1}) = x_i(t_n) + \Delta t \cdot v_i(t_n) + (\Delta t)^2
 * \frac{F_i(t_n)}{2m_i}. \f]
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param delta_t The timestep \f$ \Delta t \f$.
 */
void calculateX(ParticleContainer &particles, double delta_t);