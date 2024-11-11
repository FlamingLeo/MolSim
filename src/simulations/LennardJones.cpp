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

LennardJones::LennardJones(const std::string &filename, const Arguments &args, int type)
    : m_particles{ParticleContainer()}, m_generator{CuboidGenerator(filename, m_particles)}, m_epsilon{args.epsilon},
      m_sigma{args.sigma}, m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t},
      m_itFreq{args.itFreq}, m_type{args.type} {
    SPDLOG_TRACE("Created LJ Simulation from file {} with Arguments {}", filename, args.toString());
    initializeSimulation(type);
}
LennardJones::LennardJones(const ParticleContainer &pc, const Arguments &args, int type)
    : m_particles{pc}, m_generator{CuboidGenerator("", m_particles)}, m_epsilon{args.epsilon}, m_sigma{args.sigma},
      m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created LJ Simulation from using ParticleContainer {} with Arguments {}", pc.toString(),
                 args.toString());
    initializeSimulation(type);
}
LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::initializeSimulation(int type) {
    SPDLOG_TRACE("Initializing LJ simulation...");

    // initialize writer and physics functions
    StrategyFactory sf;
    WriterFactory wf;
    m_writer = wf.createWriter(m_type);
    auto [cv, cx, cf] = sf.getSimulationFunctions(SimulationType::LJ, type);
    m_calculateV = cv;
    m_calculateX = cx;
    m_calculateF = cf;

    // initialize particles from cuboid data
    if (m_particles.isEmpty())
        m_generator.generateCuboids();
}

void LennardJones::runSimulation() {
    SPDLOG_TRACE("Running LJ simulation (entered function)...");

    double current_time = m_startTime;
    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running LJ simulation with the following arguments:");
    SPDLOG_INFO("epsilon     : {}", m_epsilon);
    SPDLOG_INFO("sigma       : {}", m_sigma);
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    while (current_time < m_endTime) {
        m_calculateX(m_particles, m_delta_t);
        m_calculateF(m_particles, m_epsilon, m_sigma);
        m_calculateV(m_particles, m_delta_t);

        iteration++;
        if (iteration % m_itFreq == 0) {
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, current_time);
            m_writer->writeParticles(m_particles, iteration);
        }

        current_time += m_delta_t;
    }

    SPDLOG_INFO("Completed LJ simulation.");
}