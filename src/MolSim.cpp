
#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    Arguments args;
    CLIParser::parseArguments(argc, argv, args);
    std::string filename = argv[argc - 1];

    auto sim = createSimulation(args.sim, filename, args);
    sim->runSimulation();
    
    return 0;
}