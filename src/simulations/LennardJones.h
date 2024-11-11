/**
 * @file LennardJones.h
 * @brief Simulation of the Lennard-Jones potential.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "io/output/WriterFactory.h"
#include "objects/CuboidGenerator.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <memory>
#include <string>

class LennardJones : public Simulation {
  private:
    ParticleContainer m_particles;
    CuboidGenerator m_generator;
    double m_epsilon;
    double m_sigma;
    double m_startTime;
    double m_endTime;
    double m_delta_t;
    int m_itFreq;
    WriterType m_type;
    std::unique_ptr<FileWriter> m_writer;
    StrategyFactory::VFunc m_calculateV;
    StrategyFactory::XFunc m_calculateX;
    StrategyFactory::FFunc m_calculateF;

  public:
    LennardJones(const std::string &filename, const Arguments &args, int type = 0);
    LennardJones(const ParticleContainer &pc, const Arguments &args, int type = 0);
    ~LennardJones();
    void initializeSimulation(int type = 0);
    void runSimulation() override;
};
