#include "SimulationLC.h"
#include "utils/StringUtils.h"

SimulationLC::SimulationLC(ParticleContainer &pc, Arguments &args)
    : Simulation(pc, args),
      m_cellContainer{CellContainer(args.domainSize, args.conditions, args.cutoffRadius, m_particles)} {
    SPDLOG_TRACE("Created new linked cells Simulation.");
}
SimulationLC::~SimulationLC() = default;

void SimulationLC::runSimulation() {
    SPDLOG_INFO("Running {} linked cell simulation with the following arguments:",
                StringUtils::fromSimulationType(m_args.sim));
    SPDLOG_INFO("TODO!");

    initializeBase(0);
    runSimulationLoop(&m_cellContainer);

    SPDLOG_INFO("Completed {} simulation.", StringUtils::fromSimulationType(m_args.sim));
}