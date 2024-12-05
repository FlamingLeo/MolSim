#include "LennardJonesLC.h"
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

LennardJonesLC::LennardJonesLC(ParticleContainer &pc, const Arguments &args, int type)
    : m_epsilon{args.epsilon}, m_sigma{args.sigma},
      m_linkedCells{CellContainer(args.domainSize, args.conditions, args.cutoffRadius, pc)} {
    initializeBase(args, type, SimulationType::LJLC);
    SPDLOG_TRACE("Created LJLC Simulation from using ParticleContainer {} with Arguments {}", pc.toString(),
                 args.toString());
}
LennardJonesLC::~LennardJonesLC() { SPDLOG_TRACE("Destroyed LJLC object."); }

void LennardJonesLC::runSimulation() {
    SPDLOG_TRACE("Running LJLC simulation (entered function)...");

    SPDLOG_INFO("Running LJLC simulation with the following arguments:");
    SPDLOG_INFO("epsilon     : {}", m_epsilon);
    SPDLOG_INFO("sigma       : {}", m_sigma);
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("domain size : {}", ArrayUtils::to_string(m_linkedCells.getDomainSize()));
    SPDLOG_INFO("cell size   : {}", ArrayUtils::to_string(m_linkedCells.getCellSize()));
    SPDLOG_INFO("r_cutoff    : {}", m_linkedCells.getCutoff());
    SPDLOG_INFO("b.conditions: {}", CellUtils::fromBoundaryConditionArray(m_linkedCells.getConditions()));
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    runSimulationLoop(m_epsilon, m_sigma, &m_linkedCells);

    SPDLOG_INFO("Completed LJLC simulation.");
}

CellContainer &LennardJonesLC::getCellContainer() { return m_linkedCells; }