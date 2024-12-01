/**
 * @file CellContainer.h
 * @brief Class for holding a vector of Cells that divide the domain
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
    std::vector<Cell> cells;
    std::vector<Cell *> borderCells;
    std::vector<Cell *> haloCells;
    std::array<double, 3> domainSize;
    std::array<double, 3> cellSize;
    std::array<size_t, 3> numCells;
    double cutoff;
    ParticleContainer &particles;

  public:
    CellContainer(const std::array<double, 3> domainSize, double cutoff, ParticleContainer &pc);

    /// @brief Standard library iterator function for marking the beginning of the iteration process.
    /// @return An iterator pointing to the first element of cells.
    ContainerType::iterator begin();

    /// @brief Standard library iterator function for marking the end of the iteration process.
    /// @return An iterator pointing to the last element of cells.
    ContainerType::iterator end();

    /// @brief Standard library iterator function for marking the beginning of the iteration process of a const
    /// ParticleContainer.
    /// @return An iterator pointing to the first element of cells.
    ContainerType::const_iterator begin() const;

    /// @brief Standard library iterator function for marking the end of the iteration process of a const
    /// ParticleContainer.
    /// @return An iterator pointing to the last element of cells.
    ContainerType::const_iterator end() const;

    class SpecialParticleIterator {
      private:
        std::vector<Cell *>::iterator cellIt;
        std::vector<Cell *>::iterator cellEnd;
        std::forward_list<Particle *>::iterator particleIt;
        std::forward_list<Particle *>::iterator particleEnd;

        void advanceToNextCell();

      public:
        SpecialParticleIterator(std::vector<Cell *>::iterator start, std::vector<Cell *>::iterator end);
        Particle &operator*();
        SpecialParticleIterator &operator++();
        bool operator!=(const SpecialParticleIterator &other) const;
    };

    SpecialParticleIterator boundaryBegin();
    SpecialParticleIterator boundaryEnd();
    SpecialParticleIterator haloBegin();
    SpecialParticleIterator haloEnd();

    std::vector<Cell> &getCells();
    int getCellIndex(const std::array<double, 3> &position);
    void deleteParticle(Particle &p);
    bool addParticle(Particle &p);
    bool moveParticle(Particle &p);
    std::array<int, 3> getVirtualCellCoordinates(int index);
    std::vector<int> getNeighbors(int cellIndex);
    const std::array<double, 3> &getDomainSize();
    const std::array<double, 3> &getCellSize();
    const std::array<size_t, 3> &getNumCells();
    ParticleContainer &getParticles();
    double getCutoff();
    void printCellIndices();
    void printCellContents();
    ParticleContainer reconstructContainer();
};
