#include "io/input/CLIParser.h"
#include "io/input/XMLReader.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    // set log level to trace to let macro definition handle correct level
    spdlog::set_level(spdlog::level::trace);

#if defined(DO_BENCHMARKING) && SPDLOG_ACTIVE_LEVEL < 2
    // prevent running main with benchmarking options enabled
    // file output and logging are both disabled, effectively rendering the program useless
    SPDLOG_ERROR("Cannot perform benchmarking with debug log information! Recompile with SPDLOG_LEVEL greater than 1 "
                 "and try again.");
    std::exit(EXIT_FAILURE);
#endif

#ifdef NDEBUG
    // when compiling without debug settings, shorten log output to omit uninteresting information
    if (SPDLOG_ACTIVE_LEVEL > 1)
        spdlog::set_pattern("[%^%l%$] %v");
#endif

    // check for invalid syntax (not enough args)
    CLIParser::checkArgc(argc);

    // check if "-h" is passed
    // running '$ ./MolSim -h' should still be valid, even if no file input is given
    CLIParser::checkHelpString(argc, argv);

    // initialize simulation-relevant objects
    Arguments args;
    ParticleContainer pc;

    // parse xml file first, then parse command line arguments
    XMLReader r(argv[argc - 1]);
    r.readXML(args, pc);
    CLIParser::parseArguments(argc, argv, args);
    auto sim = SimulationFactory::createSimulation(pc, args);

    // run simulation with parsed arguments
    sim->runSimulation();
}