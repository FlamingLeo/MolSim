/**
 * @file CellContainer.h
 * @brief Class for holding a vector of Cells that divide a domain of Particle objects.
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Cell.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include "io/input/FileReader.h"
#include "io/output/WriterFactory.h"
#include "utils/CellUtils.h"
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// NOTE 1: you could probably use arrays instead of vectors and make this a template class,
// since we can compute the number of cells beforehand.

// NOTE 2: i only extended halo cells one cell past the boundary. theoretically, they could continue infinitely, but
// that would be a nightmare to manage.

/// @brief Cell encapsulation class for implementing the linked cell method.
class CellContainer {
    /// @brief Typedef for the underlying Cell container type.
    using ContainerType = std::vector<Cell>;

  private:
    /// @brief The container of Cell objects itself.
    ContainerType cells;
    /// @brief Container of pointers to border cells.
    std::vector<Cell *> borderCells;
    /// @brief Container of pointers to halo cells.
    std::vector<Cell *> haloCells;
    /// @brief The size of the domain in each dimension.
    std::array<double, 3> domainSize;
    /// @brief The size of each cell in each dimension (default: 0, 0, 0).
    std::array<double, 3> cellSize{0, 0, 0};
    /// @brief The number of cells in each dimension (default: 1, 1, 1).
    std::array<size_t, 3> numCells{1, 1, 1};
    /// @brief An array of boundary conditions to be applied at each boundary (North, South, West, East, Above, Below).
    std::array<BoundaryCondition, 6> conditions;
    /// @brief The cutoff radius.
    double cutoff;
    /// @brief A reference to the overarching ParticleContainer.
    ParticleContainer &particles;

  public:
    /**
     * @brief Constructs a new CellContainer.
     *
     * @param domainSize The size of the domain.
     * @param conditions The boundary conditions to be applied at each boundary.
     * @param cutoff The cutoff radius.
     * @param particles The main ParticleContainer.
     * @param dim The dimension of the container. May either be two- (2) or three-dimensional (3).
     */
    CellContainer(const std::array<double, 3> &domainSize, const std::array<BoundaryCondition, 6> &conditions,
                  double cutoff, ParticleContainer &particles, size_t dim = 2);

    /**
     * @brief Standard library iterator function for marking the beginning of the iteration process.
     *
     * @return An iterator pointing to the first element of cells.
     */
    ContainerType::iterator begin();

    /**
     * @brief Standard library iterator function for marking the end of the iteration process.
     *
     * @return An iterator pointing to the last element of cells.
     */
    ContainerType::iterator end();

    /**
     * @brief Direct access to the beginning of the ParticleContainer's PairIterator.
     *
     * Use this to iterate over all distinct pairs of Particle objects in the domain.
     *
     * @return The beginning of the ParticleContainer's PairIterator.
     */
    ParticleContainer::PairIterator beginPairs();

    /**
     * @brief Direct access to the end of the ParticleContainer's PairIterator.
     *
     * Use this to iterate over all distinct pairs of Particle objects in the domain.
     *
     * @return The end of the ParticleContainer's PairIterator.
     */
    ParticleContainer::PairIterator endPairs();

    /**
     * @brief Standard library iterator function for marking the beginning of the iteration process of a const
     * CellContainer.
     *
     * @return An iterator pointing to the first element of cells.
     */
    ContainerType::const_iterator begin() const;

    /**
     * @brief Standard library iterator function for marking the end of the iteration process of a const CellContainer.
     *
     * @return An iterator pointing to the last element of cells.
     */
    ContainerType::const_iterator end() const;

    /// @brief A special iterator for iterating over the boundary and halo cell containers.
    class SpecialParticleIterator {
      private:
        /// @brief The outer iterator, iterating over each Cell pointer.
        std::vector<Cell *>::iterator outerIt;
        /// @brief The end of the outer iterator, iterating over each Cell pointer.
        std::vector<Cell *>::iterator outerEnd;
        /// @brief The inner iterator, iterating over each Particle pointer in a Cell.
        std::forward_list<Particle *>::iterator innerIt;
        /// @brief The end of the inner iterator, iterating over each Particle pointer in a Cell.
        std::forward_list<Particle *>::iterator innerEnd;
        /// @brief Helper function to move to the next Cell which contains at least one particle. Stops when no further
        /// cells can be searched.
        void advance();

      public:
        /**
         * @brief Construct a new special iterator.
         *
         * @param start The beginning of the iterator. Automatically progresses to the first cell with at least one
         * particle.
         * @param end The end of the iterator (one past the final cell).
         */
        SpecialParticleIterator(std::vector<Cell *>::iterator start, std::vector<Cell *>::iterator end);

        /**
         * @brief Overload of the dereference operator.
         *
         * Gets a reference to the current Particle object.
         *
         * @return A reference to the current Particle object.
         */
        Particle &operator*();

        /**
         * @brief Overload of the preincrement operator.
         *
         * Either advances the inner iterator, or the outer iterator until a Cell with at least one Particle is found.
         *
         * @return The iterator again, for reuse.
         */
        SpecialParticleIterator &operator++();

        /**
         * @brief Overload of the inequality operator.
         *
         * @param other The SpecialParticleIterator to compare it to.
         * @return true if both iterators point to different objects.
         * @return false if both iterators point to the same object.
         */
        bool operator!=(const SpecialParticleIterator &other) const;

        /**
         * @brief Overload of the equality operator.
         *
         * @param other The SpecialParticleIterator to compare it to.
         * @return true if both iterators point to the same object.
         * @return false if both iterators point to different objects.
         */
        bool operator==(const SpecialParticleIterator &other) const;
    };

    /**
     * @brief Gets a SpecialParticleIterator pointing to the beginning of the boundary Cell pointer container.
     *
     * @return A SpecialParticleIterator pointing to the beginning of the boundary Cell pointer container.
     */
    SpecialParticleIterator boundaryBegin();

    /**
     * @brief Gets a SpecialParticleIterator pointing to the end of the boundary Cell pointer container.
     *
     * @return A SpecialParticleIterator pointing to the end of the boundary Cell pointer container.
     */
    SpecialParticleIterator boundaryEnd();

    /**
     * @brief Gets a SpecialParticleIterator pointing to the beginning of the halo Cell pointer container.
     *
     * @return A SpecialParticleIterator pointing to the beginning of the halo Cell pointer container.
     */
    SpecialParticleIterator haloBegin();

    /**
     * @brief Gets a SpecialParticleIterator pointing to the end of the halo Cell pointer container.
     *
     * @return A SpecialParticleIterator pointing to the end of the halo Cell pointer container.
     */
    SpecialParticleIterator haloEnd();

    /**
     * @brief Dereference of the [] operator.
     *
     * Gets a Cell by index. Does NOT perform bounds checking.
     *
     * @param index The index of the Cell to get.
     * @return A reference to the Cell at the specified index.
     */
    Cell &operator[](size_t index);

    /**
     * @brief Dereference of the [] operator (const).
     *
     * Gets a Cell by index. Does NOT perform bounds checking.
     *
     * @param index The index of the Cell to get.
     * @return A const reference to the Cell at the specified index.
     */
    const Cell &operator[](size_t index) const;

    /**
     * @brief Gets a reference to the Cell container.
     *
     * @return A reference to the Cell container.
     */
    std::vector<Cell> &getCells();

    /**
     * @brief Gets a const reference to the Cell container.
     *
     * @return A const reference to the Cell container.
     */
    const std::vector<Cell> &getCells() const;

    /**
     * @brief Gets a reference to the border Cell container.
     *
     * @return A reference to the border Cell container.
     */
    std::vector<Cell *> &getBorderCells();

    /**
     * @brief Gets a const reference to the border Cell container.
     *
     * @return A const reference to the border Cell container.
     */
    const std::vector<Cell *> &getBorderCells() const;

    /**
     * @brief Gets a reference to the halo Cell container.
     *
     * @return A reference to the halo Cell container.
     */
    std::vector<Cell *> &getHaloCells();

    /**
     * @brief Gets a const reference to the halo Cell container.
     *
     * @return A const reference to the halo Cell container.
     */
    const std::vector<Cell *> &getHaloCells() const;

    /**
     * @brief Removes the active halo Cell Particle objects.
     *
     * For each halo Cell particle, if the Particle is active, it is removed from the Cell's linked list and is marked
     * inactive.
     *
     */
    void removeHaloCellParticles();

    /**
     * @brief Gets the 1D index in the Cell container based on the given position.
     *
     * @param position The position from which to compute the corresponding 1D index.
     * @return The 1D index in the Cell container.
     */
    int getCellIndex(const std::array<double, 3> &position);

    /**
     * @brief Removes a Particle from a Cell and marks it inactive.
     *
     * First, the function finds the 1D index of the Particle's Cell. From there, it is removed, then marked as
     * inactive. Note that, in order for this function to work, the Particle must have its correct index stored, and the
     * index must be valid (i.e. not -1).
     *
     * @param p The Particle to remove.
     */
    void deleteParticle(Particle &p);

    /**
     * @brief Adds a Particle to the Cell container.
     *
     * The 1D container index is computed from the Particle's current position. If the index is valid, the Particle is
     * added there, otherwise nothing happens.
     *
     * @param p
     * @return true if the Particle was successfully added.
     * @return false if the Particle could not be added.
     */
    bool addParticle(Particle &p);

    /**
     * @brief Moves a particle from one Cell to another.
     *
     * The Particle is first deleted, then added again. The source and destionation cells may be the same. Even if the
     * Particle might not have moved, it will definitely be removed, even if something goes wrong.
     *
     * @param p
     * @return true if the Particle was successfully moved.
     * @return false if the Particle could not be moved. Here, the Particle is removed from the Cell container.
     */
    bool moveParticle(Particle &p);

    /**
     * @brief Gets the coordinates of a Cell from its index.
     *
     * @param index The index of the Cell in the container.
     * @return The lower-left origin coordinates of the corresponding Cell.
     */
    std::array<int, 3> getVirtualCellCoordinates(int index) const;

    /**
     * @brief Mirrors a position from a source Cell to a destination Cell.
     *
     * To be used primarily when incorporating reflective boundaries.
     *
     * @param position The position in the source Cell.
     * @param from The source Cell.
     * @param to The destination Cell.
     * @param direction The direction(s) in which to mirror the coordinates.
     * @return The mirrored position inside the destination Cell.
     */
    std::array<double, 3> getMirrorPosition(const std::array<double, 3> &position, const Cell &from, const Cell &to,
                                            int direction) const;

    /**
     * @brief Gets the index of the opposing Cell in the specified direction(s).
     *
     * In other words, if the direction is NORTH, this function will return the southern neighbor. If multiple
     * dimensions are passed (e.g. NORTH, WEST), the function will return the index of the opposing Cell in ALL
     * diretions (e.g. SOUTH, EAST).
     *
     * @param cellIndex The Cell from which to get the opposing Cell index.
     * @param directions A vector containing the opposite direction(s) of the desired neighbor Cell.
     * @return The index of the opposing Cell in the specified direction.
     */
    int getOppositeNeighbor(int cellIndex, const std::vector<HaloLocation> &directions) const;

    /**
     * @brief Gets a vector of neighbouring Cell indices, including the Cell itself.
     *
     * @param cellIndex The index of the Cell for which the neighbours should be determined.
     * @return A vector of neighbouring Cell indices, including the Cell itself.
     */
    std::vector<int> getNeighbors(int cellIndex) const;

    /**
     * @brief Gets a const reference to the CellContainer's domain size.
     *
     * @return A const reference to the CellContainer's domain size.
     */
    const std::array<double, 3> &getDomainSize() const;

    /**
     * @brief Gets a const reference to the CellContainer's cell size.
     *
     * @return A const reference to the CellContainer's cell size.
     */
    const std::array<double, 3> &getCellSize() const;

    /**
     * @brief Gets a const reference to the CellContainer's cell count in each dimension.
     *
     * @return A const reference to the CellContainer's cell count in each dimension.
     */
    const std::array<size_t, 3> &getNumCells() const;

    /**
     * @brief Gets the boundary conditions at each boundary.
     *
     * @return A reference to the array containing the boundary conditions at each boundary.
     */
    const std::array<BoundaryCondition, 6> &getConditions() const;

    /**
     * @brief Gets the cutoff radius used for calculating the cell size.
     *
     * @return The cutoff radius.
     */
    double getCutoff() const;

    /**
     * @brief Gets a reference to the primary ParticleContainer.
     *
     * @return A reference to the overarching ParticleContainer.
     */
    ParticleContainer &getParticles();

    /**
     * @brief Gets a const reference to the primary ParticleContainer.
     *
     * @return A const reference to the overarching ParticleContainer.
     */
    const ParticleContainer &getParticles() const;

    /**
     * @brief Gets the size of the ParticleContainer, including all inactive Cells.
     *
     * @return The total size of the ParticleContainer.
     */
    size_t size() const;

    /**
     * @brief Gets the amount of active Particle objects in the ParticleContainer.
     *
     * @return The total number of active Particle objects.
     */
    size_t activeSize() const;

    /**
     * @brief Debug function to print the indices of all cells for a 2D container.
     *
     * Halo cells are marked red, border cells are marked yellow, inner cells are marked green.
     *
     */
    void printCellIndices() const;

    /**
     * @brief Debug function to print the contents of each cell.
     *
     */
    void printCellContents() const;
};
