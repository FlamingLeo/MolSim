/**
 * @file PositionCalculation.h
 * @brief Strategy functions for calculating the position of one or more particles.
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/CellContainer.h"
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
 * @param lc Unused. Present to allow calling function without last argument.
 */
void calculateX(ParticleContainer &particles, double delta_t, CellContainer *lc = nullptr);

/**
 * @brief Calculates the position \f$ x \f$ for all Particle objects in a given ParticleContainer when using the linked
 * cells method.
 *
 * @details The position \f$ x_i \f$ of a given particle \f$ i \f$ at the next unit of time \f$ t_{n+1} \f$ is
 * calculated using the formula \f[ x_i(t_{n+1}) = x_i(t_n) + \Delta t \cdot v_i(t_n) + (\Delta t)^2
 * \frac{F_i(t_n)}{2m_i}. \f]
 *
 * After each update, the particle may need to be moved to a different cell. The algorithm checks this and updates the
 * cell correspondence accordingly.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param delta_t The timestep \f$ \Delta t \f$.
 * @param lc The CellContainer for the linked cells method.
 */
void calculateX_LC(ParticleContainer &particles, double delta_t, CellContainer *lc);