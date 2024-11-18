#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "io/input/XMLReader.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/StringUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char *argv[]) {
    // set log level to trace to let macro definition handle correct level
    spdlog::set_level(spdlog::level::trace);

#ifdef DO_BENCHMARKING
    // prevent running main with benchmarking options enabled
    // file output and logging are both disabled, effectively rendering the program useless
    std::cerr << "[" << BOLD_ON << "error" << BOLD_OFF
              << "] Cannot run main program in benchmark mode! Recompile the program and try again.\n";
    std::exit(EXIT_FAILURE);
#endif

#ifdef NDEBUG
    // when compiling without debug settings, shorten log output to omit uninteresting information
    if (SPDLOG_ACTIVE_LEVEL > 1)
        spdlog::set_pattern("[%^%l%$] %v");
#endif

    std::string filename = argv[argc - 1];

    // initialize simulation-relevant objects
    Arguments args;
    ParticleContainer pc;

    // parse XML input file
    XMLReader r(filename);
    r.readXML(args, pc);

    // parse command line arguments
    CLIParser::parseArguments(argc, argv, args);

    // run desired simulation based on user choice
    auto sim = SimulationFactory::createSimulation(args.sim, pc, args);
    sim->runSimulation();
}