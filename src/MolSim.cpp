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
    /* when compiling without debug settings, shorten log output to omit uninteresting information */
    spdlog::set_pattern("[%^%l%$] %v");
#endif

    // parse command line arguments
    Arguments args;
    CLIParser::parseArguments(argc, argv, args);
    std::string filename = argv[argc - 1];

    // run desired simulation based on user choice
    auto sim = createSimulation(args.sim, filename, args);
    sim->runSimulation();
}