#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/StringUtils.h"
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char *argv[]) {
    // set log level to trace to let macro definition handle correct level
    spdlog::set_level(spdlog::level::trace);

#ifdef NDEBUG
    // when compiling without debug settings, shorten log output to omit uninteresting information
    if (SPDLOG_ACTIVE_LEVEL > 1)
        spdlog::set_pattern("[%^%l%$] %v");
#endif

    // parse command line arguments
    std::string filename = argv[argc - 1];

    Arguments args;
    CLIParser::parseArguments(argc, argv, args);

    // run desired simulation based on user choice
    auto sim = SimulationFactory::createSimulation(args.sim, filename, args);
    sim->runSimulation();
}