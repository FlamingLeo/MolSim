#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "objects/ParticleContainer.h"
#include "simulations/SimulationFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/StringUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include "objects/LinkedCells.h"

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

    // parse command line arguments
    std::string filename = argv[argc - 1];

    Arguments args;
    CLIParser::parseArguments(argc, argv, args);

    // run desired simulation based on user choice
    auto sim = SimulationFactory::createSimulation(args.sim, filename, args);
    sim->runSimulation();

    /*
    std::cout << "working\n";
    (void)argc;
    (void)argv;

    double currentTime = 0;
    double endTime = 20;
    double delta_t = 0.0005;
    int iteration = 0;
    int m_totalIt = (int) endTime / delta_t;
    WriterType m_type = WriterType::VTK;
    std::unique_ptr<FileWriter> m_writer = WriterFactory::createWriter(m_type);

    LinkedCells lc = LinkedCells();
    std::cout<< lc.getCells().size() << " cells\n";
    for(Cell &c : lc.getCells()){
        for(Particle p : c.getParticles()){
            std::cout<<" start pos " << p.getX() << "\n";
        }
    }

    while (currentTime < endTime) {
        lc.calculatePosition();
        lc.calculateForce();
        lc.calculateVelocity();

        iteration++;

        if (iteration % 10 == 0) {
            std::cout<<"iteration " << iteration << "\n";
            ParticleContainer pp = ParticleContainer();
            for(Cell &c : lc.getCells()){
                for(Particle &p : c.getParticles()){
                    pp.addParticle(p);
                    std::cout<<"pos " << p.getX() << "\n";
                }
            }
            std::cout<<pp.toString()<< "\n";
            m_writer->writeParticles(pp, iteration, m_totalIt);
        }

        currentTime += delta_t;
    }
    */
    return 0;
}