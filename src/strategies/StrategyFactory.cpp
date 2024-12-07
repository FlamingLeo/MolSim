#include "StrategyFactory.h"
#include "ForceCalculation.h"
#include "PositionCalculation.h"
#include "VelocityCalculation.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <tuple>

static std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> getSimulationFunctions_nonLC(SimulationType type,
                                                                                             int modifier) {
    SPDLOG_DEBUG("Getting physics functions for non-linked cell simulation...");
    switch (type) {
    case SimulationType::GRAVITY:
        SPDLOG_DEBUG("Chose physics calculations for gravitational simulation with force calculation: {}",
                     modifier ? "Naive" : "Newton's Third Law");
        return std::make_tuple(TimeIntegrationFuncs(type, false),
                               modifier ? calculateF_Gravity : calculateF_GravityThirdLaw);
    case SimulationType::LJ:
        StrategyFactory::FFunc f;
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
        return std::make_tuple(TimeIntegrationFuncs(type, false), f);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(type, false), calculateF_LennardJones); // stop compiler warnings
}

static std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> getSimulationFunctions_LC(SimulationType type,
                                                                                          int modifier) {
    SPDLOG_DEBUG("Getting physics functions for linked cell simulation...");
    switch (type) {
    case SimulationType::GRAVITY:
        CLIUtils::error("Linked cells method is currently unsupported with gravitational simulation!", "", false);
        break;
    case SimulationType::LJ:
        StrategyFactory::FFunc f;
        switch (modifier) {
        case 0:
            f = calculateF_LennardJonesThirdLaw_LC;
            SPDLOG_DEBUG(
                "Chose physics calculations for linked cell LJ simulation with force calculation: Newton's Third Law");
            break;
        case 1:
            f = calculateF_LennardJones_LC;
            SPDLOG_DEBUG("Chose physics calculations for linked cell LJ simulation with force calculation: Naive");
            break;
        default:
            CLIUtils::error("Unknown function modifier!", "", false);
            break;
        }
        return std::make_tuple(TimeIntegrationFuncs(type, true), f);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(type, true), calculateF_LennardJones_LC); // stop compiler warnings
}

TimeIntegrationFuncs::TimeIntegrationFuncs(SimulationType type, bool lc) {
    switch (type) {
    case SimulationType::GRAVITY:
    case SimulationType::LJ:
        vf = calculateV;
        xf = lc ? calculateX_LC : calculateX;
        break;
    default:
        CLIUtils::error("Unknown type!", "", false);
    }
}

std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> StrategyFactory::getSimulationFunctions(Arguments &args,
                                                                                                 int modifier) {
    return args.linkedCells ? getSimulationFunctions_LC(args.sim, modifier)
                            : getSimulationFunctions_nonLC(args.sim, modifier);
}