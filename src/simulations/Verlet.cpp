#include "Verlet.h"
#include "io/output/WriterFactory.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <string>

Verlet::Verlet(const ParticleContainer &pc, const Arguments &args)
    : m_particles{pc}, m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t},
      m_itFreq{args.itFreq}, m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with ParticleContainer {} and Arguments {}", pc.toString(),
                 args.toString());
}
Verlet::Verlet(const std::string &filename, const Arguments &args)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    m_particles.fromFile(filename);
    SPDLOG_TRACE("Created Verlet simulation from file {} with Arguments {}", filename, args.toString());
}
Verlet::Verlet(const Arguments &args)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with Arguments {}", args.toString());
}
Verlet::~Verlet() { SPDLOG_TRACE("Destroyed Verlet object."); }

void Verlet::runSimulation() {
    SPDLOG_TRACE("Running Verlet simulation (entered function)...");

    if (m_particles.isEmpty())
        CLIUtils::error("Cannot run simulation without particles!", "", false);

    // initialize writer and physics functions
    StrategyFactory sf;
    WriterFactory wf;
    auto writer = wf.createWriter(m_type);
    auto [calculateV, calculateX, calculateF] = sf.getSimulationFunctions(SimulationType::VERLET);

    double current_time = m_startTime;
    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running Verlet simulation with the following arguments:");
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < m_endTime) {
        calculateX(m_particles, m_delta_t);
        calculateF(m_particles, 0, 0);
        calculateV(m_particles, m_delta_t);

        iteration++;
        if (iteration % m_itFreq == 0) {
            // logging done here because otherwise the console output would legitimately become unreadable
            // this also makes it somewhat more configurable at runtime
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, current_time);
            writer->writeParticles(m_particles, iteration);
        }

        current_time += m_delta_t;
    }

    SPDLOG_INFO("Completed Verlet simulation.");
}

ParticleContainer &Verlet::getParticles() { return m_particles; }