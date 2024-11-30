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
#define TOTAL static_cast<int>((args.endTime - args.startTime) / args.delta_t)

LennardJones::LennardJones(const std::string &filename, const Arguments &args, int type)
    : m_particles{ParticleContainer()}, m_generator{CuboidGenerator(filename, m_particles)}, m_epsilon{args.epsilon},
      m_sigma{args.sigma}, m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t},
      m_itFreq{args.itFreq}, m_totalIt{TOTAL}, m_type{args.type}, m_basename{args.basename} {
    SPDLOG_TRACE("Created LJ Simulation from file {} with Arguments {}", filename, args.toString());
    initializeSimulation(type);
}
LennardJones::LennardJones(const ParticleContainer &pc, const Arguments &args, int type)
    : m_particles{pc}, m_generator{CuboidGenerator("", m_particles)}, m_epsilon{args.epsilon}, m_sigma{args.sigma},
      m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_totalIt{TOTAL}, m_type{args.type}, m_basename{args.basename} {
    SPDLOG_TRACE("Created LJ Simulation from using ParticleContainer {} with Arguments {}", pc.toString(),
                 args.toString());
    initializeSimulation(type);
}
LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::initializeSimulation(int type) {
    SPDLOG_TRACE("Initializing LJ simulation...");

    // initialize writer and physics functions
    m_writer = WriterFactory::createWriter(m_type, m_basename);
    auto [cvx, cf] = StrategyFactory::getSimulationFunctions(SimulationType::LJ, type);
    m_calculateV = cvx.vf;
    m_calculateX = cvx.xf;
    m_calculateF = cf;

    // initialize particles from cuboid data
    if (m_particles.isEmpty())
        m_generator.generateCuboids();
}

void LennardJones::runSimulation() {
    SPDLOG_TRACE("Running LJ simulation (entered function)...");

    // verify that we have something to work with (compiled out on release builds)
    assert(!(m_particles.isEmpty()) && "Cannot run simulation without particles!");

    double currentTime = m_startTime;
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

    while (currentTime < m_endTime) {
        m_calculateX(m_particles, m_delta_t);
        m_calculateF(m_particles, m_epsilon, m_sigma);
        m_calculateV(m_particles, m_delta_t);

        iteration++;

#ifndef DO_BENCHMARKING
        if (iteration % m_itFreq == 0) {
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, currentTime);
            m_writer->writeParticles(m_particles, iteration, m_totalIt);
        }
#endif

        currentTime += m_delta_t;
    }

    SPDLOG_INFO("Completed LJ simulation.");
}

ParticleContainer &LennardJones::getParticles() { return m_particles; }