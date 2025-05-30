#include "SimulationLC.h"
#include "io/output/XMLWriter.h"
#include "utils/ArrayUtils.h"
#include "utils/CellUtils.h"
#include "utils/OMPWrapper.h"
#include "utils/StringUtils.h"

SimulationLC::SimulationLC(ParticleContainer &pc, Arguments &args, Thermostat &t, FlowSimulationAnalyzer &analyzer)
    : Simulation(pc, args, t, analyzer),
      m_cellContainer{
          CellContainer(args.domainSize, args.conditions, args.cutoffRadius, m_particles, args.dimensions)} {
    SPDLOG_TRACE("Created new linked cells Simulation.");
}
SimulationLC::~SimulationLC() = default;

void SimulationLC::runSimulation() {
    CHECK_NOUTFLOW(m_args, conditions);
    SPDLOG_INFO("Running {} linked cell simulation with the following arguments:",
                StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("start time  : {}", m_args.startTime);
    SPDLOG_INFO("end time    : {}", m_args.endTime);
    SPDLOG_INFO("timestep    : {}", m_args.delta_t);
    SPDLOG_INFO("output freq.: {}", m_args.itFreq);
    SPDLOG_INFO("dimensions  : {}", m_args.dimensions);
    SPDLOG_INFO("domain size : {}", ArrayUtils::to_string(m_args.domainSize));
    SPDLOG_INFO("cell size   : {}", ArrayUtils::to_string(m_cellContainer.getCellSize()));
    SPDLOG_INFO("cutoff      : {}", m_args.cutoffRadius);
    SPDLOG_INFO("gravity     : {}", m_args.gravity);
    SPDLOG_INFO("basename    : {}", m_args.basename);
    SPDLOG_INFO("output type : {}", StringUtils::fromWriterType(m_args.type));
    SPDLOG_INFO("bd. cond.   : {}", CellUtils::fromBoundaryConditionArray(m_args.conditions));
    SPDLOG_INFO("#particles  : {}", m_particles.size());
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
    runSimulationLoop(&m_cellContainer);

    // serialize output for future runs
    SIM_SERIALIZE_XML(m_args.basename + "_results.xml", m_particles, m_args, m_thermostat, m_analyzer);

    SPDLOG_INFO("Completed {} linked cell simulation.", StringUtils::fromSimulationType(m_args.sim));
}