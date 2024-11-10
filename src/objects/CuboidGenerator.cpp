//
// Created by marag on 11/5/2024.
//
#include "CuboidGenerator.h"
#include "Cuboid.h"
#include "io/input/FileReader.h"
#include <iostream>
#include <string>
#include <vector>

CuboidGenerator::CuboidGenerator(const ParticleContainer &particles) { (void)particles; }

CuboidGenerator::CuboidGenerator(const std::string &filename, const ParticleContainer &particles)
    : m_filename{filename}, particles{particles} {};

std::vector<Cuboid> CuboidGenerator::generateCuboids() {
    // read arguments from file
    // create cuboid and call initializeParticles()
    // add cuboid to cuboids vec
    std::vector<Cuboid> cuboids;
    FileReader f(m_filename);
    f.readFile(cuboids, particles);
    return cuboids;
}

/*
x1 v1 ...
x2 v2 ...
*/