#include "Simulation.h"
#include "io/output/XMLWriter.h"
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
    // verify that the particle container is not empty
    assert(!(m_particles.isEmpty()) && "Cannot run simulation without particles!");

#ifdef DO_BENCHMARKING
    SPDLOG_WARN("Benchmarking ENABLED! No output files will be generated.");
#endif

    // reset (start) timer
    TIMER_RESET(m_timer);

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

        // add the number of active particles to the molecule update counter
        // we do this since we update each active molecule during one iteration
        TIMER_UPDATE_MOLECULES(m_timer, m_particles.activeSize());
    }

    // print total elapsed time and molecule updates per second
    TIMER_PRINT_ELAPSED(m_timer);
    TIMER_PRINT_MUPS(m_timer);
}

void Simulation::runSimulation() {
    SPDLOG_INFO("Running {} simulation with the following arguments:", StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("start time  : {}", m_args.startTime);
    SPDLOG_INFO("end time    : {}", m_args.endTime);
    SPDLOG_INFO("timestep    : {}", m_args.delta_t);
    SPDLOG_INFO("output freq.: {}", m_args.itFreq);
    SPDLOG_INFO("basename    : {}", m_args.basename);
    SPDLOG_INFO("output type : {}", StringUtils::fromWriterType(m_args.type));

    initializeBase(0);          // we should let the user choose this...
    runSimulationLoop(nullptr); // "nullptr" isn't necessary here, but it shows the diff between this and lc

    // serialize output for future runs
    XMLWriter xmlw{m_args.basename + "_results.xml"};
    xmlw.serialize(m_particles, m_args);

    SPDLOG_INFO("Completed {} simulation.", StringUtils::fromSimulationType(m_args.sim));
}

ParticleContainer &Simulation::getParticles() { return m_particles; }