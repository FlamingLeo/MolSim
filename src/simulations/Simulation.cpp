#include "Simulation.h"
#include "utils/OMPWrapper.h"
#include "utils/StringUtils.h"
#include <algorithm>

Simulation::Simulation(ParticleContainer &pc, Arguments &args, Thermostat &t, FlowSimulationAnalyzer &analyzer)
    : m_particles{pc}, m_args{args}, m_thermostat{t}, m_analyzer{analyzer} {
    SPDLOG_TRACE("Created new Simulation.");
}
Simulation::~Simulation() = default;

void Simulation::initializeBase() {
    // get total number of iterations
    m_totalIt = static_cast<int>((m_args.endTime - m_args.startTime) / m_args.delta_t);

    // initialize output writer
    SIM_INIT_WRITER(m_writer, m_args.type, m_args.basename);

    // initialize physics functions
    auto [cvx, cf] = StrategyFactory::getSimulationFunctions(m_args);
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
    int iteration = 0;

    while (currentTime < m_args.endTime) {
        SPDLOG_DEBUG("Iteration: {}", iteration);

        // compute statistics of the flow of the nano scale flow simulation
        SIM_ANALYZE_FLOW(m_analyzer, iteration);

        // update system temperature using thermostat
        m_thermostat.updateSystemTemp(iteration);

        // update position, force and velocity
        m_calculateX(m_particles, m_args.delta_t, m_args.gravity, lc, m_args.membrane);
        m_calculateF(m_particles, m_args.cutoffRadius, lc);
        m_calculateV(m_particles, m_args.delta_t);

        // for standard builds, generate output files
        SIM_WRITE_OUTPUT(iteration, m_args.itFreq, m_writer, m_particles, m_totalIt);

        currentTime += m_args.delta_t;

        // add the number of active particles to the molecule update counter
        // we do this since we update each active molecule during one iteration
        TIMER_UPDATE_MOLECULES(m_timer, m_particles.activeSize());
        iteration++;
    }

    // print total elapsed time and molecule updates per second
    TIMER_PRINT_ELAPSED(m_timer);
    TIMER_PRINT_MUPS(m_timer);
}

void Simulation::runSimulation() {
    CHECK_NOUTFLOW(m_args, conditions);
    SPDLOG_INFO("Running {} simulation with the following arguments:", StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("start time  : {}", m_args.startTime);
    SPDLOG_INFO("end time    : {}", m_args.endTime);
    SPDLOG_INFO("timestep    : {}", m_args.delta_t);
    SPDLOG_INFO("output freq.: {}", m_args.itFreq);
    SPDLOG_INFO("basename    : {}", m_args.basename);
    SPDLOG_INFO("output type : {}", StringUtils::fromWriterType(m_args.type));
    SPDLOG_INFO("nanoflow?   : {}", m_thermostat.getNanoflow());
    SPDLOG_INFO("membrane?   : {}", m_args.membrane);
    SPDLOG_INFO("analyzer?   : {}", m_analyzer.getFrequency() > 0);
    if (m_analyzer.getFrequency() > 0) {
        SPDLOG_INFO("a: bin nr.  : {}", m_analyzer.getBinNumber());
        SPDLOG_INFO("a: lwall    : {}", m_analyzer.getLeftWallPosX());
        SPDLOG_INFO("a: rwall    : {}", m_analyzer.getRightWallPosX());
        SPDLOG_INFO("a: freq.    : {}", m_analyzer.getFrequency());
        SPDLOG_INFO("a: dirname  : {}", m_analyzer.getDirname());
    }
#ifdef _OPENMP
    SPDLOG_INFO("p. strat.   : {}", StringUtils::fromParallelizationType(m_args.parallelization));
    SPDLOG_INFO("max threads : {}", omp_get_max_threads());
#else
    SPDLOG_WARN("Parallelization is DISABLED!");
#endif

    initializeBase();
    runSimulationLoop(nullptr); // "nullptr" isn't necessary here, but it shows the diff between this and lc

    // serialize output for future runs
    SIM_SERIALIZE_XML(m_args.basename + "_results.xml", m_particles, m_args, m_thermostat, m_analyzer);

    SPDLOG_INFO("Completed {} simulation.", StringUtils::fromSimulationType(m_args.sim));
}

ParticleContainer &Simulation::getParticles() { return m_particles; }