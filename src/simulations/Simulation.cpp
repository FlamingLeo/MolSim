#include "Simulation.h"
#include "utils/StringUtils.h"

Simulation::Simulation(ParticleContainer &pc, Arguments &args) : m_particles{pc}, m_args{args} {
    SPDLOG_TRACE("Created new Simulation.");
}
Simulation::~Simulation() = default;

void Simulation::initializeBase(int type) {
    // get total number of iterations
    m_totalIt = static_cast<int>((m_args.endTime - m_args.startTime) / m_args.delta_t);

    // initialize output writer
    m_writer = WriterFactory::createWriter(m_args.type, m_args.basename);

    // initialize physics functions
    auto [cvx, cf] = StrategyFactory::getSimulationFunctions(m_args, type);
    m_calculateX = cvx.xf;
    m_calculateV = cvx.vf;
    m_calculateF = cf;
}

void Simulation::runSimulationLoop(CellContainer *lc) {
#if 0
    if (lc) {
        SPDLOG_TRACE("Found linked cell container.");
    }
    ParticleContainer &container = lc ? lc->getParticles() : m_particles;
#endif

    // verify that the particle container is not empty
    assert(!(m_particles.isEmpty()) && "Cannot run simulation without particles!");

    // perform time integration
    double currentTime = m_args.startTime;
#ifndef DO_BENCHMARKING
    int iteration = 0;
#endif

    while (currentTime < m_args.endTime) {
        m_calculateX(m_particles, m_args.delta_t, lc);
        m_calculateF(m_particles, m_args.cutoffRadius, lc);
        m_calculateV(m_particles, m_args.delta_t);

#ifndef DO_BENCHMARKING
        iteration++;
        if (iteration % m_args.itFreq == 0) {
            m_writer->writeParticles(m_particles, iteration, m_totalIt);
        }
#endif
        currentTime += m_args.delta_t;
    }
}

void Simulation::runSimulation() {
    SPDLOG_INFO("Running {} simulation with the following arguments:", StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("TODO!");

    initializeBase(0);          // we should let the user choose this...
    runSimulationLoop(nullptr); // "nullptr" isn't necessary here, but it shows the diff between this and lc

    SPDLOG_INFO("Completed {} simulation.", StringUtils::fromSimulationType(m_args.sim));
}

ParticleContainer &Simulation::getParticles() { return m_particles; }