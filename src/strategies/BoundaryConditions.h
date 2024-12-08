/**
 * @file BoundaryConditions.h
 * @brief Utility (strategy?) functions for applying boundary conditions.
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "objects/Cell.h"
#include "objects/CellContainer.h"
#include "objects/Particle.h"
#include "utils/CellUtils.h"

/**
 * @brief Handles the scenario where a particle enters a halo cell. Returns true if the particle is handled
 * and further processing can be skipped, false otherwise.
 *
 * @param p The particle that has moved.
 * @param targetCell The target cell that needs to be checked.
 * @param lc The CellContainer containing the cell.
 * @return true if the cell is a halo cell and a suitable boundary condition was applied.
 * @return false if the cell is not a halo cell or if a suitable boundary condition could not be applied.
 */
bool handleHaloCell(Particle &p, Cell &targetCell, CellContainer *lc);

/**
 * @brief Determines the boundary condition for a particle entering a halo cell.
 *
 * @param p The particle entering a halo cell.
 * @param targetCell The halo cell which the particle enters.
 * @param lc The CellContainer containing the cell.
 * @return The applied boundary condition.
 */
BoundaryCondition determineBoundaryCondition(Particle &p, Cell &targetCell, CellContainer *lc);

/**
 * @brief Handles the outflow condition where the particle is deleted upon entering a halo cell.
 *
 * @param p The particle entering a halo cell.
 * @param targetCell The halo cell which the particle enters.
 * @param lc The CellContainer containing the cell.
 */
void handleOutflowCondition(Particle &p, Cell &targetCell, CellContainer *lc);

/**
 * @brief Handles the reflective condition where the particle is mirrored and moved to the opposite cell.
 *
 * @param p The particle entering a halo cell.
 * @param fromCell The current cell of the particle.
 * @param lc The CellContainer containing the cell.
 */
void handleReflectiveCondition(Particle &p, Cell &fromCell, CellContainer *lc);

/**
 * @brief Handles the periodic condition where the particle is moved from the halo cell to the appropriate border cell
 * on the other side.
 *
 * @param p The particle entering a halo cell.
 * @param fromCell The current cell of the particle.
 * @param lc The CellContainer containing the cell.
 */
void handlePeriodicCondition(Particle &p, Cell &targetCell, CellContainer *lc);

/**
 * @brief Reflects a particle by mirroring its position and moving it to a new cell.
 *
 * @param p The particle entering a halo cell.
 * @param fromCell The current cell of the particle. Passed to the getMirrorPosition() function in CellContainer.
 * @param toCell The target cell of the particle. Passed to the getMirrorPosition() function in CellContainer.
 * @param lc The CellContainer containing the cell.
 * @param dimension The dimension argument passed to the getMirrorPosition() function in CellContainer.
 */
void reflectParticle(Particle &p, Cell &fromCell, Cell &toCell, CellContainer *lc, int dimension);