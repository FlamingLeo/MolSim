//
// Created by marag on 11/5/2024.
//
#pragma once
#include "../objects/CuboidGenerator.h"
#include "Simulation.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <string>

class LennardJones : public Simulation {
  private:
    ParticleContainer m_particles;
    CuboidGenerator m_generator;
    int m_epsilon;        // default: 5
    int m_sigma;          // default: 1
    double m_currentTime; // default: 0
    double m_endTime;     // default: 5
    double m_delta_t;     // default: 0.0002
    int m_itFreq;
    WriterType m_type;

  public:
    LennardJones(const std::string &filename, const Arguments &args);
    ~LennardJones();
    void runSimulation() override;
    void LJForce();
    void calculateX();
    void calculateV();
};
