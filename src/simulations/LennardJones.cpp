//
// Created by marag on 11/5/2024.
//
#include "LennardJones.h"
#include <spdlog/spdlog.h>

LennardJones::LennardJones(const std::string &filename, const Arguments &args)
    : particles{ParticleContainer()}, generator{CuboidGenerator(particles)} {
    SPDLOG_TRACE("Created LJ Simulation");
};

LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); };

void LennardJones::runSimulation(){};