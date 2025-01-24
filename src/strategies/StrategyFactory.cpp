#include "StrategyFactory.h"
#include "ForceCalculation.h"
#include "PositionCalculation.h"
#include "VelocityCalculation.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <tuple>

static inline std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc>
getSimulationFunctions_nonLC(SimulationType type) {
    SPDLOG_DEBUG("Getting physics functions for non-linked cell simulation...");
    switch (type) {
    case SimulationType::GRAVITY:
        SPDLOG_DEBUG("Chose physics calculations for gravitational simulation.");
        return std::make_tuple(TimeIntegrationFuncs(type, false), calculateF_Gravity);
    case SimulationType::LJ:
        SPDLOG_DEBUG("Chose physics calculations for Lennard-Jones simulation.");
        return std::make_tuple(TimeIntegrationFuncs(type, false), calculateF_LennardJones);
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(type, false), calculateF_LennardJones); // stop compiler warnings
}


static inline std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> getSimulationFunctions_LC(Arguments &args) {
    SPDLOG_DEBUG("Getting physics functions for linked cell simulation...");
    switch (args.sim) {
    case SimulationType::GRAVITY:
        CLIUtils::error("Linked cells method is currently unsupported with gravitational simulation!", "", false);
        break;
    case SimulationType::LJ:
        SPDLOG_DEBUG("Chose physics calculations for linked-cell Lennard-Jones simulation.");

        if (args.parallelization == ParallelizationType::COARSE) {
            SPDLOG_DEBUG("Chose coarse-grained (standard) parallelization strategy.");
            if(args.membrane){
                return std::make_tuple(TimeIntegrationFuncs(args.sim, true), calculateF_Membrane_LC);
            }
            return std::make_tuple(TimeIntegrationFuncs(args.sim, true), calculateF_LennardJones_LC);

        } else {
            if(args.membrane){
                return std::make_tuple(TimeIntegrationFuncs(args.sim, true), calculateF_Membrane_LC);
            }
            SPDLOG_DEBUG("Chose fine-grained (task-based) parallelization strategy.");
            return std::make_tuple(TimeIntegrationFuncs(args.sim, true), calculateF_LennardJones_LC_task);
        }
        break;
    default:
        CLIUtils::error("Invalid simulation type!");
    }
    return std::make_tuple(TimeIntegrationFuncs(args.sim, true), calculateF_LennardJones_LC); // stop compiler warnings
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

std::tuple<TimeIntegrationFuncs, StrategyFactory::FFunc> StrategyFactory::getSimulationFunctions(Arguments &args) {

    return args.linkedCells ? getSimulationFunctions_LC(args) : getSimulationFunctions_nonLC(args.sim);

}