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
    : m_generator(filename, m_particles), m_epsilon{args.epsilon}, m_sigma{args.sigma} {
    initializeBase(args, type, SimulationType::LJ);
    if (m_particles.isEmpty())
        m_generator.generateCuboids();
    SPDLOG_TRACE("Created LJ Simulation from file {} with Arguments {}", filename, args.toString());
}
LennardJones::LennardJones(const ParticleContainer &pc, const Arguments &args, int type)
    : m_generator("", m_particles), m_epsilon{args.epsilon}, m_sigma{args.sigma} {
    m_particles = pc;
    initializeBase(args, type, SimulationType::LJ);
    SPDLOG_TRACE("Created LJ Simulation from using ParticleContainer {} with Arguments {}", pc.toString(),
                 args.toString());
}
LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::runSimulation() {
    SPDLOG_TRACE("Running LJ simulation (entered function)...");

    SPDLOG_INFO("Running LJ simulation with the following arguments:");
    SPDLOG_INFO("epsilon     : {}", m_epsilon);
    SPDLOG_INFO("sigma       : {}", m_sigma);
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    runSimulationLoop(m_epsilon, m_sigma);

    SPDLOG_INFO("Completed LJ simulation.");
}