//
// Created by marag on 11/5/2024.
//

#ifndef PSEMOLDYN_GROUPC_CUBOIDGENERATOR_H
#define PSEMOLDYN_GROUPC_CUBOIDGENERATOR_H

#endif // PSEMOLDYN_GROUPC_CUBOIDGENERATOR_H

#include "Cuboid.h"
#include "ParticleContainer.h"
#include <vector>

class CuboidGenerator {
  private:
    std::string m_filename;
  public:
    CuboidGenerator(const ParticleContainer &particles);
    CuboidGenerator(const std::string &filename);
    std::vector<Cuboid> generateCuboids();
};