/**
 * @file LinkedCells.h
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
// that would be (1) useless and (2) a nightmare to manage.

class LinkedCells {
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
    LinkedCells(const std::array<double, 3> domainSize, double cutoff, ParticleContainer &pc);
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
    double getCutoff();
    void printCellIndices();
    void printCellContents();
    ParticleContainer reconstructContainer();
};
