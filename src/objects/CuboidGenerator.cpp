#include "CuboidGenerator.h"
#include "Cuboid.h"
#include "io/input/FileReader.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

CuboidGenerator::CuboidGenerator(const std::string &filename, ParticleContainer &particles)
    : m_filename{filename}, m_particles{particles} {};

void CuboidGenerator::generateCuboids() {
    assert(m_particles.isEmpty() && "Particle container should be empty!");

    FileReader f(m_filename);
    f.readCuboids(m_particles);
}