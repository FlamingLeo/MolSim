#include "Gravity.h"
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

Gravity::Gravity(ParticleContainer &pc, const Arguments &args, int type) {
    m_particles = pc;
    initializeBase(args, type, SimulationType::GRAVITY);
    SPDLOG_TRACE("Created Gravity simulation with ParticleContainer {} and Arguments {}", pc.toString(),
                 args.toString());
}
Gravity::Gravity(const Arguments &args, int type) {
    initializeBase(args, type, SimulationType::GRAVITY);
    SPDLOG_TRACE("Created Gravity simulation with Arguments {}", args.toString());
}
Gravity::~Gravity() { SPDLOG_TRACE("Destroyed Gravity object."); }

void Gravity::runSimulation() {
    SPDLOG_TRACE("Running Gravity simulation (entered function)...");

    SPDLOG_INFO("Running Gravity simulation with the following arguments:");
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    runSimulationLoop();

    SPDLOG_INFO("Completed Gravity simulation.");
}