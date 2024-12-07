#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "io/input/XMLReader.h"
#include "objects/CellContainer.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "strategies/ForceCalculation.h"
#include "strategies/PositionCalculation.h"
#include "strategies/VelocityCalculation.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/PathUtils.h"
#include "utils/StringUtils.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
namespace fs = std::filesystem;

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