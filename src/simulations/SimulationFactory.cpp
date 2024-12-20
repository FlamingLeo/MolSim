#include "SimulationFactory.h"
#include "utils/CLIUtils.h"
#include <memory>
#include <string>

std::unique_ptr<Simulation> SimulationFactory::createSimulation(ParticleContainer &pc, Arguments &args, Thermostat &t) {
    return args.linkedCells ? std::make_unique<SimulationLC>(pc, args, t) : std::make_unique<Simulation>(pc, args, t);
}