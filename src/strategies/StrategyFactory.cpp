#include "StrategyFactory.h"
#include "F.h"
#include "V.h"
#include "X.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <tuple>

std::tuple<StrategyFactory::VFunc, StrategyFactory::XFunc, StrategyFactory::FFunc>
StrategyFactory::getSimulationFunctions(SimulationType type, int modifier) {
    switch (type) {
    case SimulationType::GRAVITY:
        /*
        modifier values:
        0 - calculate force using newton's third law
        1 - calculate force using naive approach
        */
        SPDLOG_DEBUG("Chose physics calculations for gravitational simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(calculateV, calculateX, modifier ? calculateF_Gravity : calculateF_GravityThirdLaw);
    case SimulationType::LJ:
        /*
        modifier values (TBD):
        0 - calculate force using newton's third law
        1 - calculate force using naive approach
        */
        SPDLOG_DEBUG("Chose physics calculations for LJ simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(calculateV, calculateX,
                               modifier ? calculateF_LennardJones : calculateF_LennardJonesThirdLaw);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(calculateV, calculateX, calculateF_LennardJones); // stop compiler warnings
}