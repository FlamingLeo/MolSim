/**
 * @file LinkedCells.h
 * @brief Class for holding a vector of Cells that divide the domain
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "io/input/FileReader.h"
#include "io/output/WriterFactory.h"
#include "strategies/ForceCalculation.h"
#include "strategies/VelocityCalculation.h"
#include "strategies/PositionCalculation.h"
#include "Cell.h"
#include "Particle.h"
#include "ParticleContainer.h"
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class LinkedCells {

  private:
    std::vector<Cell> cells;

    std::vector<Cell *> border_cells;
    std::vector<Cell *> halo_cells;

    std::array<double, 3> domainSize;
    std::array<double, 3> cellSize;
    std::array<int, 3> numCells;
    double cutoff;

  public:
    LinkedCells();

    std::vector<Cell> &getCells();

    int getCellIndex(const std::array<double, 3> &position);

    void deleteParticle(Particle &p);

    void addParticle(const Particle &particle);

    std::array<int, 3> getVirtualCellCoordinates(int index);

    std::vector<int> getNeighbors3D(int cellIndex);

    void calculateVelocity();
    void calculateForce();
    void calculatePosition();
};
