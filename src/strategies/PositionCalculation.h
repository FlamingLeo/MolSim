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
 * As an optimization, in order to prevent looping through all particles once more to reset the existing force
 * and potentially apply gravity in the upcoming force calculation step of the time integration, this is done
 * at the end of this function instead. If the gravity is set to 0, the force applied to all active particles
 * will be zeroed instead.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param delta_t The timestep \f$ \Delta t \f$.
 * @param g_grav The gravitational force \f$ g_{grav} \f$.
 * @param lc Unused. Present to allow calling function without last argument.
 * @param membrane Unused. Present to allow calling function without last argument.
 */
void calculateX(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *lc = nullptr,
                bool membrane = false);

/**
 * @brief Calculates the position \f$ x \f$ for all Particle objects in a given ParticleContainer when using the linked
 * cells method.
 *
 * @details The position update occurs analogously to the default, non-linked-cell function.
 *
 * After each update, the particle may need to be moved to a different cell. The algorithm checks this and updates the
 * cell correspondence accordingly. If the particle enters a halo cell, the appropriate boundary condition will be
 * applied.
 *
 * If a particle enters a corner halo cell where one side has a different boundary condition to the other, the condition
 * is chosen based on which boundary the particle will hit first. See the report and presentation for more details.
 *
 * As an optimization, in order to prevent looping through all particles once more to reset the existing force
 * and potentially apply gravity in the upcoming force calculation step of the time integration, this is done
 * at the end of this function instead. If the gravity is set to 0, the force applied to all active particles
 * will be zeroed instead.
 *
 * @param particles The ParticleContainer containing the Particle objects to iterate over.
 * @param delta_t The timestep \f$ \Delta t \f$.
 * @param g_grav The gravitational force \f$ g_{grav} \f$.
 * @param lc The CellContainer for the linked cells method.
 * @param membrane Determines, whether gravity should be applied along the z-axis (true) or y-axis (false).
 */
void calculateX_LC(ParticleContainer &particles, double delta_t, double g_grav, CellContainer *lc,
                   bool membrane = false);