#include "SimulationFactory.h"
#include "utils/CLIUtils.h"
#include <memory>
#include <string>

std::unique_ptr<Simulation> SimulationFactory::createSimulation(ParticleContainer &pc, Arguments &args, Thermostat &t, FlowSimulationAnalyzer &analyzer) {
    return args.linkedCells ? std::make_unique<NanoScaleFlowSim>(pc, args, t, analyzer) : std::make_unique<Simulation>(pc, args, t, analyzer);
}