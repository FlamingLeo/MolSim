#include "StrategyFactory.h"
#include "F.h"
#include "V.h"
#include "X.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <tuple>

std::tuple<StrategyFactory::VFunc, StrategyFactory::XFunc, StrategyFactory::FFunc>
StrategyFactory::getSimulationFunctions(SimulationType type) {
    switch (type) {
    case SimulationType::VERLET:
        SPDLOG_DEBUG("Chose physics calculations for Verlet simulation.");
        return std::make_tuple(calculateV, calculateX, calculateF_VerletThirdLaw);
    case SimulationType::LJ:
        SPDLOG_DEBUG("Chose physics calculations for LJ simulation.");
        return std::make_tuple(calculateV, calculateX, calculateF_LennardJones);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(calculateV, calculateX, calculateF_LennardJones); // stop compiler warnings
}