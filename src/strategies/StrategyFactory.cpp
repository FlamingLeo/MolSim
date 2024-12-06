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
        vf = calculateV;
        xf = calculateX;
        break;
    case SimulationType::LJLC:
        vf = calculateV;
        xf = calculateX_LC;
        break;
    default:
        CLIUtils::error("Unknown type!", "", false);
    }
}

std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> StrategyFactory::getSimulationFunctions(SimulationType type,
                                                                                                 int modifier) {
    switch (type) {
    case SimulationType::GRAVITY:
        SPDLOG_DEBUG("Chose physics calculations for gravitational simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(TimeIntegrationFuncs(type), modifier ? calculateF_Gravity : calculateF_GravityThirdLaw);
    case SimulationType::LJ:
        FFunc f;
        switch (modifier) {
        case 0:
            f = calculateF_LennardJonesThirdLaw;
            SPDLOG_DEBUG("Chose physics calculations for LJ simulation with force calculation: Newton's Third Law");
            break;
        case 1:
            f = calculateF_LennardJones;
            SPDLOG_DEBUG("Chose physics calculations for LJ simulation with force calculation: Naive");
            break;
        case 2:
            f = calculateF_LennardJonesThirdLawCutoff;
            SPDLOG_DEBUG(
                "Chose physics calculations for LJ simulation with force calculation: Newton's Third Law (Cutoff)");
            break;
        case 3:
            f = calculateF_LennardJonesCutoff;
            SPDLOG_DEBUG("Chose physics calculations for LJ simulation with force calculation: Naive (Cutoff)");
            break;
        default:
            CLIUtils::error("Unknown function modifier!", "", false);
            break;
        }

        return std::make_tuple(TimeIntegrationFuncs(type), f);
    case SimulationType::LJLC:
        SPDLOG_DEBUG("Chose physics calculations for LJLC simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(TimeIntegrationFuncs(type),
                               modifier ? calculateF_LennardJones_LC : calculateF_LennardJonesThirdLaw_LC);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(type), calculateF_LennardJones); // stop compiler warnings
}