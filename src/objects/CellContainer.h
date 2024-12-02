/**
 * @file CellContainer.h
 * @brief Class for holding a vector of Cells that divide the domain.
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
class CellContainer {
    using ContainerType = std::vector<Cell>;

  private:
    ContainerType cells;
    std::vector<Cell *> borderCells;
    std::vector<Cell *> haloCells;
    std::array<double, 3> domainSize;
    std::array<double, 3> cellSize;
    std::array<size_t, 3> numCells;
    std::array<BoundaryCondition, 6> conditions;
    double cutoff;
    ParticleContainer &particles;

  public:
    CellContainer(const std::array<double, 3> &domainSize, const std::array<BoundaryCondition, 6> &conditions,
                  double cutoff, ParticleContainer &particles, size_t dim = 2);

    /// @brief Standard library iterator function for marking the beginning of the iteration process.
    /// @return An iterator pointing to the first element of cells.
    ContainerType::iterator begin();

    /// @brief Standard library iterator function for marking the end of the iteration process.
    /// @return An iterator pointing to the last element of cells.
    ContainerType::iterator end();

    ParticleContainer::PairIterator beginPairs();
    ParticleContainer::PairIterator endPairs();

    /// @brief Standard library iterator function for marking the beginning of the iteration process of a const
    /// ParticleContainer.
    /// @return An iterator pointing to the first element of cells.
    ContainerType::const_iterator begin() const;

    /// @brief Standard library iterator function for marking the end of the iteration process of a const
    /// CellContainer.
    /// @return An iterator pointing to the last element of cells.
    ContainerType::const_iterator end() const;

    class SpecialParticleIterator {
      private:
        std::vector<Cell *>::iterator outerIt;
        std::vector<Cell *>::iterator outerEnd;
        std::forward_list<Particle *>::iterator innerIt;
        std::forward_list<Particle *>::iterator innerEnd;
        void advance();

      public:
        SpecialParticleIterator(std::vector<Cell *>::iterator start, std::vector<Cell *>::iterator end);
        Particle &operator*();
        SpecialParticleIterator &operator++();
        bool operator!=(const SpecialParticleIterator &other) const;
        bool operator==(const SpecialParticleIterator &other) const;
    };

    SpecialParticleIterator boundaryBegin();
    SpecialParticleIterator boundaryEnd();
    SpecialParticleIterator haloBegin();
    SpecialParticleIterator haloEnd();

    Cell &operator[](size_t index);
    const Cell &operator[](size_t index) const;

    std::vector<Cell> &getCells();
    const std::vector<Cell> &getCells() const;
    std::vector<Cell *> &getBorderCells();
    const std::vector<Cell *> &getBorderCells() const;
    std::vector<Cell *> &getHaloCells();
    const std::vector<Cell *> &getHaloCells() const;

    void removeHaloCells();
    int getCellIndex(const std::array<double, 3> &position);
    void deleteParticle(Particle &p);
    bool addParticle(Particle &p);
    bool moveParticle(Particle &p);
    std::array<int, 3> getVirtualCellCoordinates(int index) const;
    std::array<double, 3> getMirrorPosition(const std::array<double, 3> &position, const Cell &from, const Cell &to,
                                            int direction) const;
    int getOppositeNeighbor(int cellIndex, const std::vector<HaloLocation> &directions) const;
    std::vector<int> getNeighbors(int cellIndex) const;
    const std::array<double, 3> &getDomainSize() const;
    const std::array<double, 3> &getCellSize() const;
    const std::array<size_t, 3> &getNumCells() const;
    const std::array<BoundaryCondition, 6> &getConditions() const;
    double getCutoff() const;
    ParticleContainer &getParticles();
    const ParticleContainer &getParticles() const;
    size_t size() const;
    size_t activeSize() const;
    void printCellIndices() const;
    void printCellContents() const;
};
