//
// Created by marag on 11/5/2024.
//
#pragma once

#include "Cuboid.h"
#include "ParticleContainer.h"
#include <string>
#include <vector>

class CuboidGenerator {
  private:
    std::string m_filename;
    ParticleContainer &particles;

  public:
    CuboidGenerator(const std::string &filename, ParticleContainer &particles);
    std::vector<Cuboid> generateCuboids();
};