#include "SimulationFactory.h"
#include "utils/CLIUtils.h"

std::unique_ptr<Simulation> createSimulation(SimulationType type, const Arguments& args) {
    switch(type) {
        case SimulationType::VERLET: return std::make_unique<Verlet>(args);
        default: CLIUtils::error("Invalid Simulation type!");
    }
}