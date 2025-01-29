#include "io/input/CLIParser.h"
#include "io/input/XMLReader.h"
#include "objects/ParticleContainer.h"
#include "objects/Thermostat.h"
#include "objects/FlowSimulationAnalyzer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    // set log level to trace to let macro definition handle correct level
    spdlog::set_level(spdlog::level::trace);

#if defined(DO_BENCHMARKING) && defined(DO_PROFILING)
    // prevent benchmarking and profiling simultaneously to avoid measuring I/O when timing runtime
    CLIUtils::error("Cannot do benchmarking and profiling at the same time!", "", false);
#endif

#if defined(DO_PROFILING) && SPDLOG_ACTIVE_LEVEL < 6
    // prevent profiling with logging enabled
    CLIUtils::error("Cannot do profiling with logging turned on!", "", false);
#endif

#if defined(DO_BENCHMARKING) && SPDLOG_ACTIVE_LEVEL < 2
    // prevent benchmarking with debug logging information
    CLIUtils::error("Cannot perform benchmarking with debug log information!", "", false);
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
    Thermostat t{pc};

    // parse xml file first, then parse command line arguments
    XMLReader r(argv[argc - 1]);
    r.readXML(args, pc, t);
    CLIParser::parseArguments(argc, argv, args);

    // create simulation and run with parsed arguments
    FlowSimulationAnalyzer analyzer = FlowSimulationAnalyzer(pc, 50, 4.0, 30.2, 10);
    auto sim = SimulationFactory::createSimulation(pc, args, t, analyzer);
    sim->runSimulation();
}