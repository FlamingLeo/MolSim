//
// Created by marag on 11/5/2024.
//
#include "CuboidGenerator.h"
#include "Cuboid.h"
#include "io/input/FileReader.h"
#include <iostream>
#include <string>
#include <vector>

CuboidGenerator::CuboidGenerator(const std::string &filename, ParticleContainer &particles)
    : m_filename{filename}, m_particles{particles} {};

void CuboidGenerator::generateCuboids() {
    FileReader f(m_filename);
    f.readCuboids(m_particles);
}