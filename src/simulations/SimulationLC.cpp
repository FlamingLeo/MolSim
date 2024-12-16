#include "SimulationLC.h"
#include "io/output/XMLWriter.h"
#include "utils/ArrayUtils.h"
#include "utils/CellUtils.h"
#include "utils/StringUtils.h"

SimulationLC::SimulationLC(ParticleContainer &pc, Arguments &args, Thermostat &t)
    : Simulation(pc, args, t),
      m_cellContainer{CellContainer(args.domainSize, args.conditions, args.cutoffRadius, m_particles)} {
    SPDLOG_TRACE("Created new linked cells Simulation.");
}
SimulationLC::~SimulationLC() = default;

void SimulationLC::runSimulation() {
    SPDLOG_INFO("Running {} linked cell simulation with the following arguments:",
                StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("start time  : {}", m_args.startTime);
    SPDLOG_INFO("end time    : {}", m_args.endTime);
    SPDLOG_INFO("timestep    : {}", m_args.delta_t);
    SPDLOG_INFO("output freq.: {}", m_args.itFreq);
    SPDLOG_INFO("domain size : {}", ArrayUtils::to_string(m_args.domainSize));
    SPDLOG_INFO("cell size   : {}", ArrayUtils::to_string(m_cellContainer.getCellSize()));
    SPDLOG_INFO("cutoff      : {}", m_args.cutoffRadius);
    SPDLOG_INFO("gravity     : {}", m_args.gravity);
    SPDLOG_INFO("basename    : {}", m_args.basename);
    SPDLOG_INFO("output type : {}", StringUtils::fromWriterType(m_args.type));
    SPDLOG_INFO("bd. cond.   : {}", CellUtils::fromBoundaryConditionArray(m_args.conditions));
    SPDLOG_INFO("#particles  : {}", m_particles.size());

    initializeBase(0);
    runSimulationLoop(&m_cellContainer);

    // serialize output for future runs
    XMLWriter xmlw{m_args.basename + "_results.xml"};
    xmlw.serialize(m_particles, m_args, m_thermostat);

    SPDLOG_INFO("Completed {} simulation.", StringUtils::fromSimulationType(m_args.sim));
}