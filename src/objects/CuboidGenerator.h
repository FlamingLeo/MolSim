/**
 * @file CuboidGenerator.h
 * @brief Generator class for creating and initializing Cuboid objects.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Cuboid.h"
#include "ParticleContainer.h"
#include <string>
#include <vector>

/// @brief CuboidGenerator class handling the initialization of all Cuboids.
class CuboidGenerator {
  private:
    /// @brief Filename of the input file.
    std::string m_filename = "";

    /// @brief Reference to the central ParticleContainer to which all particles are added.
    ParticleContainer &m_particles;

  public:
    /**
     * @brief Construct a new CuboidGenerator object.
     *
     * @param filename  A reference to the string representing the input filename.
     * @param particles A reference to the ParticleContainer for the particles in the cuboid(s).
     */
    CuboidGenerator(const std::string &filename, ParticleContainer &particles);

    /// @brief Generates all cuboid particles from an input file. Uses an underlying FileReader.
    void generateCuboids();
};