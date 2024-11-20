#include "SimulationFactory.h"
#include "utils/CLIUtils.h"
#include <memory>
#include <string>

std::unique_ptr<Simulation> SimulationFactory::createSimulation(SimulationType type, const std::string &filename,
                                                                const Arguments &args) {
    switch (type) {
    case SimulationType::GRAVITY:
        SPDLOG_DEBUG("Generating Gravity...");
        return std::make_unique<Gravity>(filename, args);
    case SimulationType::LJ:
        SPDLOG_DEBUG("Generating LJ...");
        return std::make_unique<LennardJones>(filename, args);
    default:
        CLIUtils::error("Invalid Simulation type!", "", false);
    }
    return std::make_unique<Gravity>(filename, args);
}