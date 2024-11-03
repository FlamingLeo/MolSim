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

    // parse command line arguments
    Arguments args;
    CLIParser::parseArguments(argc, argv, args);
    std::string filename = argv[argc - 1];

    // log user choices
    SPDLOG_INFO("Running {} simulation with the following arguments:", StringUtils::fromSimulationType(args.sim));
    SPDLOG_INFO("t_0         : {}", args.startTime);
    SPDLOG_INFO("t_end       : {}", args.endTime);
    SPDLOG_INFO("dt          : {}", args.delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", args.itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(args.type));

    // run desired simulation based on user choice
    auto sim = createSimulation(args.sim, filename, args);
    sim->runSimulation();

    SPDLOG_INFO("Finished running {} simulation.", StringUtils::fromSimulationType(args.sim));
}