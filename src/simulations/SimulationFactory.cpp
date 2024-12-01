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
    case SimulationType::LJLC:
        SPDLOG_DEBUG("Generating LJLC...");
        return std::make_unique<LennardJonesLC>(filename, args);
    default:
        CLIUtils::error("Invalid Simulation type!", "", false);
    }
    return std::make_unique<Gravity>(filename, args);
}

std::unique_ptr<Simulation> SimulationFactory::createSimulation(SimulationType type, const ParticleContainer &pc,
                                                                const Arguments &args) {
    switch (type) {
    case SimulationType::GRAVITY:
        SPDLOG_DEBUG("Generating Gravity...");
        return std::make_unique<Gravity>(pc, args);
    case SimulationType::LJ:
        SPDLOG_DEBUG("Generating LJ...");
        return std::make_unique<LennardJones>(pc, args);
    case SimulationType::LJLC:
        SPDLOG_DEBUG("Generating LJLC...");
        return std::make_unique<LennardJonesLC>(pc, args);
    default:
        CLIUtils::error("Invalid Simulation type!", "", false);
    }
    return std::make_unique<Gravity>(pc, args);
}