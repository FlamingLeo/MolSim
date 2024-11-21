#include "StrategyFactory.h"
#include "ForceCalculation.h"
#include "PositionCalculation.h"
#include "VelocityCalculation.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <tuple>

TimeIntegrationFuncs::TimeIntegrationFuncs(SimulationType type) {
    switch (type) {
    case SimulationType::GRAVITY:
    case SimulationType::LJ:
    default:
        vf = calculateV;
        xf = calculateX;
    }
}

std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> StrategyFactory::getSimulationFunctions(SimulationType type,
                                                                                                 int modifier) {
    switch (type) {
    case SimulationType::GRAVITY:
        /*
        modifier values:
        0 - calculate force using newton's third law
        1 - calculate force using naive approach
        */
        SPDLOG_DEBUG("Chose physics calculations for gravitational simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(TimeIntegrationFuncs(type), modifier ? calculateF_Gravity : calculateF_GravityThirdLaw);
    case SimulationType::LJ:
        /*
        modifier values:
        0 - calculate force using newton's third law
        1 - calculate force using naive approach
        */
        SPDLOG_DEBUG("Chose physics calculations for LJ simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(TimeIntegrationFuncs(type),
                               modifier ? calculateF_LennardJones : calculateF_LennardJonesThirdLaw);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(type), calculateF_LennardJones); // stop compiler warnings
}