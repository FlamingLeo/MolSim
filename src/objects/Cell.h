/**
 * @file Cell.h
 * @brief A single Cell in a CellContainer, for use with the linked cells method.
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Particle.h"
#include "utils/CellUtils.h"
#include <algorithm>
#include <array>
#include <bitset>
#include <forward_list>
#include <sstream>
#include <string>
#include <vector>
#define VEC_CONTAINS(vec, el) ((std::find((vec).begin(), (vec).end(), (el)) != (vec).end()))

/// @brief Class for storing data of a single Cell in a domain split into cells.
class Cell {
    /// @brief Typedef for the container type used to store pointers to a Cell's Particle objects.
    using ContainerType = std::forward_list<std::reference_wrapper<Particle>>;

  private:
    /// @brief A forward list of pointers to Particle objects contained within the current Cell.
    ContainerType m_particles{};
    /// @brief The size of the Cell in each dimension.
    std::array<double, 3> m_size;
    /// @brief The lower-left coordinates of the Cell.
    std::array<double, 3> m_position;
    /// @brief If this is a halo cell, the locations of this cell (North, South, West, East, Above, Below) are stored
    /// here.
    std::vector<HaloLocation> m_haloLocation;
    /// @brief If this is a border cell, the locations of this cell (North, South, West, East, Above, Below) are stored
    /// here.
    std::vector<BorderLocation> m_borderLocation;
    /// @brief The neighboring Cell indices.
    std::vector<int> m_neighbors;
    /// @brief The type of this Cell. May be INNER, BORDER or HALO.
    CellType m_type;
    /// @brief The index of this Cell in the overarching CellContainer.
    int m_index;

  public:
    /* constructor */
    /**
     * @brief Constructs a new Cell object.
     *
     * Typically called from a CellContainer.
     *
     * @param size The size of the Cell in each dimension.
     * @param position The lower-left coordinates of the Cell.
     * @param type The type of this Cell.
     * @param index The index of this Cell in the CellContainer.
     * @param haloLocation The cardinal direction(s) of this cell if this is a halo Cell.
     * @param borderLocation The cardinal direction(s) of this cell if this is a border Cell.
     */
    Cell(const std::array<double, 3> &size, const std::array<double, 3> &position, CellType type, int index,
         const std::vector<HaloLocation> &haloLocation, const std::vector<BorderLocation> &borderLocation);

    /* iterator */
    /**
     * @brief Gets an iterator to the beginning of the Particle* forward list.
     *
     * @return An iterator to the beginning of the Cell's forward list.
     */
    ContainerType::iterator begin();

    /**
     * @brief Gets an iterator to one past the end of the Particle* forward list.
     *
     * @return An iterator to one past the end of the Cell's forward list.
     */
    ContainerType::iterator end();

    /* corner cells */
    /**
     * @brief Calculates the relative position of a particle within the current cell.
     *
     * @param p The particle whose relative position is to be calculated.
     * @return An array representing the relative position of the particle within the cell.
     */
    std::array<double, 3> getRelativePosition(const Particle &p) const;

    /**
     * @brief Validates if the provided relative position is within the bounds of the current cell (debug).
     *
     * This function uses the `assert` macro. As such, it has no effect in `Release` builds.
     *
     * @param relPos The relative position to be validated, represented as a 3D array of doubles.
     */
    void validatePosition(const std::array<double, 3> &relPos) const;

    /**
     * @brief Checks if two HaloLocation values correspond to a corner.
     *
     * This function determines if the two given `HaloLocation` values represent opposite corners of the cell.
     *
     * @param loc1 The first HaloLocation to check.
     * @param loc2 The second HaloLocation to check.
     * @return true if the two locations represent a corner.
     * @return false if the two locations do not represent a corner.
     */
    bool isCorner(HaloLocation loc1, HaloLocation loc2) const;

    /**
     * @brief Handles the logic for 2D corner cell handling.
     *
     * This function handles the determination of the corner region in a 2D context based on the relative position
     * within the cell. It returns the appropriate HaloLocation (NORTH, SOUTH, EAST, or WEST) based on the position.
     *
     * See the report for Worksheet 3 for more info.
     *
     * @param relPos The relative position of the particle in 2D within the cell, represented as a 3D array.
     * @return The appropriate HaloLocation for the 2D corner (e.g., NORTH, SOUTH, EAST, WEST).
     */
    HaloLocation handle2DCorner(const std::array<double, 3> &relPos) const;

    /**
     * @brief Handles the logic for 3D corner cell handling.
     *
     * This function handles the determination of the corner region in a 3D context based on the relative position
     * within the cell. It returns the appropriate HaloLocation based on the position in the 3D corner regions.
     *
     * See the report for Worksheet 5 for a detailed explanation.
     *
     * @param relPos The relative position of the particle in 3D within the cell, represented as a 3D array.
     * @return The appropriate HaloLocation for the 3D corner (e.g., NORTH, SOUTH, EAST, WEST, ABOVE, BELOW).
     */
    HaloLocation handle3DCorner(const std::array<double, 3> &relPos) const;

    /**
     * @brief Handles the logic for determining the 3D diagonal region.
     *
     * This function handles the determination of the diagonal region in a 3D context, where the relative position
     * determines if the corner is on the diagonal. It returns the appropriate HaloLocation based on the diagonal logic.
     *
     * See the report for Worksheet 5 for a detailed explanation.
     *
     * @param relPos The relative position of the particle in 3D within the cell, represented as a 3D array.
     * @return The appropriate HaloLocation for the 3D diagonal region.
     */
    HaloLocation handle3DDiagonal(const std::array<double, 3> &relPos) const;

    /**
     * @brief Handles the logic for determining the 3D triple corner region.
     *
     * This function handles the 3D triple corner region case, where the relative position is used to determine the
     * specific triple corner region in 3D (e.g. NORTH, WEST and ABOVE). It returns the appropriate HaloLocation for the
     * triple corner.
     *
     * See the report for Worksheet 5 for a detailed explanation.
     *
     * @param relPos The relative position of the particle in 3D within the cell, represented as a 3D array.
     * @return The appropriate HaloLocation for the 3D triple corner region.
     */
    HaloLocation handle3DTripleCorner(const std::array<double, 3> &relPos) const;

    /* main functionality */
    /**
     * @brief Adds a Particle reference to the front of the forward list.
     *
     * @param particle The Particle reference to be added.
     */
    void addParticle(Particle &particle);

    /**
     * @brief Removes a Particle reference from the forward list.
     *
     * @param particle The Particle reference to be removed.
     */
    void removeParticle(Particle &particle);

    /**
     * @brief Dispatch function to handle a corner cell.
     *
     * This function calls the above corner cell routines to determine which boundary conditon should be applied based
     * on the Particle's position inside the corner cell, if it even is inside a corner cell.
     *
     * @param p The Particle to analyze.
     * @return The singular cardinal direction of the Halo Cell based on the Particle's position.
     */
    HaloLocation getCornerRegion(const Particle &p) const;

    /**
     * @brief Gets a const reference to the position array of this Cell.
     *
     * @return A const reference to the position array of this Cell.
     */
    const std::array<double, 3> &getX() const;

    /**
     * @brief Gets a const reference to the size array of this Cell.
     *
     * @return A const reference to the size array of this Cell.
     */
    const std::array<double, 3> &getSize() const;

    /**
     * @brief Gets a const reference to the halo location (cardinal direction) vector of this Cell.
     *
     * @return A const reference to the halo location vector of this Cell.
     */
    const std::vector<HaloLocation> &getHaloLocation() const;

    /**
     * @brief Gets a const reference to the border location (cardinal direction) vector of this Cell.
     *
     * @return A const reference to the border location vector of this Cell.
     */
    const std::vector<BorderLocation> &getBorderLocation() const;

    /**
     * @brief Gets a reference to the Cell's neighbors.
     *
     * @return A const reference to the Cell's neighbors.
     */
    std::vector<int> &getNeighbors();

    /**
     * @brief Gets a const reference to the Cell's neighbors.
     *
     * @return A const reference to the Cell's neighbors.
     */
    const std::vector<int> &getNeighbors() const;

    /**
     * @brief Gets a reference to the Cell's Particle pointer forward list.

     * @return A reference to the Cell's Particle pointer forward list.
     */
    std::forward_list<std::reference_wrapper<Particle>> &getParticles();

    /**
     * @brief Gets a const reference to the Cell's Particle pointer forward list.
     *
     * @return A const reference to the Cell's Particle pointer forward list.
     */
    const std::forward_list<std::reference_wrapper<Particle>> &getParticles() const;

    /**
     * @brief Gets the type of this Cell.
     *
     * @return The type of this Cell.
     */
    CellType getType() const;

    /**
     * @brief Gets the index of this Cell in its CellContainer.
     *
     * @return The index of this Cell in its CellContainer.
     */
    int getIndex() const;

    /**
     * @brief Gets a string representation of this Cell.
     *
     * @return A string representation of the Cell data.
     */
    std::string toString() const;
};