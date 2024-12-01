#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "io/input/XMLReader.h"
#include "objects/LinkedCells.h"
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
#include <spdlog/spdlog.h>
#include <string>
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
#if 1
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
    if (argc < 2)
        CLIUtils::error("Not enough arguments! Use '-h' to display a help message.");

    std::string filename = argv[argc - 1];
    std::unique_ptr<Simulation> sim;

    // initialize simulation-relevant objects
    Arguments args;
    ParticleContainer pc;

    // check if the input file is an xml file, otherwise use text file initialization methods
    if (PathUtils::isXmlFile(filename)) {
        SPDLOG_DEBUG("Input file is an XML file. Command line arguments will have precedence.");
        XMLReader r(filename);
        r.readXML(args, pc);
        CLIParser::parseArguments(argc, argv, args);
        sim = SimulationFactory::createSimulation(args.sim, pc, args);
    } else {
        SPDLOG_DEBUG("Input file is unspecified or NOT an XML file.");
        CLIParser::parseArguments(argc, argv, args);
        sim = SimulationFactory::createSimulation(args.sim, filename, args);
    }

    // run simulation with parsed arguments
    sim->runSimulation();
#else
    (void)argc, (void)argv;
    spdlog::set_level(spdlog::level::info);
    Arguments args;
    ParticleContainer pc;
    std::filesystem::path cwd = std::filesystem::current_path();
    XMLReader r(PathUtils::getTargetPath(cwd, "MolSim") + "/input/input-lj-w3t2.xml");
    r.readXML(args, pc);
    LinkedCells lc{args.domainSize, args.cutoffRadius, pc};
    VTKWriter w;
    double currentTime = args.startTime;
    int iteration = 0;

    while (currentTime < args.endTime) {
        calculateX_LC(lc, pc, args.delta_t);
        calculateF_LennardJones_LC(lc, pc, args.epsilon, args.sigma);
        calculateV(pc, args.delta_t);

        iteration++;

        if (iteration % 100 == 0) {
            w.writeParticles(pc, iteration, static_cast<int>((args.endTime) / (args.delta_t)));
        }

        currentTime += args.delta_t;
    }
#endif
}