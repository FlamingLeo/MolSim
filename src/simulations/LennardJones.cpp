//
// Created by marag on 11/5/2024.
//
#include "LennardJones.h"
#include "io/output/WriterFactory.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

LennardJones::LennardJones(const std::string &filename, const Arguments &args)
    : m_particles{ParticleContainer()}, m_generator{CuboidGenerator(filename, m_particles)}, m_epsilon{5}, m_sigma{1},
      m_currentTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created LJ Simulation from file {} with Arguments {}", filename, args.toString());
}
LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::runSimulation() {
    SPDLOG_TRACE("Running LennardJones simulation (entered function)...");

    // initialize writer and physics functions
    StrategyFactory sf;
    WriterFactory wf;
    auto writer = wf.createWriter(m_type);
    auto [calculateV, calculateX, calculateF] = sf.getSimulationFunctions(SimulationType::LJ);

    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running LJ simulation with the following arguments:");
    SPDLOG_INFO("epsilon     : {}", m_epsilon);
    SPDLOG_INFO("sigma       : {}", m_sigma);
    SPDLOG_INFO("t_0         : {}", m_currentTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    // initialize particles from cuboid data
    m_generator.generateCuboids();

    while (m_currentTime < m_endTime) {
        calculateX(m_particles, m_delta_t);
        calculateF(m_particles, m_epsilon, m_sigma);
        calculateV(m_particles, m_delta_t);

        iteration++;
        if (iteration % m_itFreq == 0) {
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, m_currentTime);
            writer->writeParticles(m_particles, iteration);
        }

        m_currentTime += m_delta_t;
    }

    SPDLOG_INFO("Completed LJ simulation.");
}