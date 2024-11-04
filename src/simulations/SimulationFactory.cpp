#include "SimulationFactory.h"
#include "utils/CLIUtils.h"
#include <memory>
#include <string>

std::unique_ptr<Simulation> createSimulation(SimulationType type, const std::string &filename, const Arguments &args) {
    switch (type) {
    case SimulationType::VERLET:
        SPDLOG_DEBUG("Generating Verlet...");
        return std::make_unique<Verlet>(filename, args);
    default:
        CLIUtils::error("Invalid Simulation type!", "", false);
    }
    return std::make_unique<Verlet>(filename, args);
}