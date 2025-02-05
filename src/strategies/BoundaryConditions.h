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
#include <utility>

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
 * @brief Determines the correct halo location and boundary condition for a particle entering a halo cell.
 *
 * @param p The particle entering a halo cell.
 * @param targetCell The halo cell which the particle enters.
 * @param lc The CellContainer containing the cell.
 * @return The chosen cardinal direction and the applied boundary condition.
 */
std::pair<HaloLocation, BoundaryCondition> determineBoundaryCondition(Particle &p, Cell &targetCell, CellContainer *lc);

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
 * @param location The cardinal direction of the entered halo cell.
 * @param lc The CellContainer containing the cell.
 */
void handleReflectiveCondition(Particle &p, Cell &fromCell, HaloLocation location, CellContainer *lc);

/**
 * @brief Handles the periodic condition where the particle is moved from the halo cell to the appropriate border cell
 * on the other side.
 *
 * @param p The particle entering a halo cell.
 * @param targetCell The cell the particle has moved to before this function is called.
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
 * @param direction The direction argument passed to the getMirrorPosition() function in CellContainer.
 */
void reflectParticle(Particle &p, Cell &fromCell, Cell &toCell, CellContainer *lc, int direction);

/**
 * @brief Adds ghost particles to the proper halo cells for periodic conditions.
 *
 * @param lc The CellContainer we are operating in.
 */
void mirrorGhostParticles(CellContainer *lc);

/**
 * @brief Deletes references to ghost particles from the halo cells (i.e all particle references in halo cells).
 *
 * @param lc The CellContainer we are operating in.
 */
void deleteGhostParticles(CellContainer *lc);