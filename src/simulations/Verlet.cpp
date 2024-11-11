#include "Verlet.h"
#include "io/output/WriterFactory.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <functional>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

Verlet::Verlet(const ParticleContainer &pc, const Arguments &args, int type)
    : m_particles{pc}, m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t},
      m_itFreq{args.itFreq}, m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with ParticleContainer {} and Arguments {}", pc.toString(),
                 args.toString());
    initializeSimulation(type);
}
Verlet::Verlet(const std::string &filename, const Arguments &args, int type)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    m_particles.fromFile(filename);
    SPDLOG_TRACE("Created Verlet simulation from file {} with Arguments {}", filename, args.toString());
    initializeSimulation(type);
}
Verlet::Verlet(const Arguments &args, int type)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with Arguments {}", args.toString());
    initializeSimulation(type);
}
Verlet::~Verlet() { SPDLOG_TRACE("Destroyed Verlet object."); }

void Verlet::initializeSimulation(int type) {
    SPDLOG_TRACE("Initializing Verlet simulation...");

    StrategyFactory sf;
    m_writer = WriterFactory::createWriter(m_type);
    auto [cv, cx, cf] = StrategyFactory::getSimulationFunctions(SimulationType::VERLET, type);
    m_calculateV = cv;
    m_calculateX = cx;
    m_calculateF = cf;
}

void Verlet::runSimulation() {
    SPDLOG_TRACE("Running Verlet simulation (entered function)...");

    // verify that we have something to work with (compiled out on release builds)
    assert(!(m_particles.isEmpty()) && "Cannot run simulation without particles!");

    double currentTime = m_startTime;
    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running Verlet simulation with the following arguments:");
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    // for this loop, we assume: current x, current f and current v are known
    while (currentTime < m_endTime) {
        m_calculateX(m_particles, m_delta_t);
        m_calculateF(m_particles, 0, 0);
        m_calculateV(m_particles, m_delta_t);

        iteration++;
        if (iteration % m_itFreq == 0) {
            // logging done here because otherwise the console output would legitimately become unreadable
            // this also makes it somewhat more configurable at runtime
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, currentTime);
            m_writer->writeParticles(m_particles, iteration);
        }

        currentTime += m_delta_t;
    }

    SPDLOG_INFO("Completed Verlet simulation.");
}

ParticleContainer &Verlet::getParticles() { return m_particles; }