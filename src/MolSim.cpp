
#include "objects/ParticleContainer.h"
#include "io/input/CLIParser.h"
#include "io/input/FileReader.h"
#include "simulations/Verlet.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

int main(int argc, char *argv[]) {
    Arguments args;
    CLIParser::parseArguments(argc, argv, args);

    // this will change when more simulations come
    Verlet v{args};

    // TODO move this into ParticleContainer, preferably
    FileReader fileReader(argv[argc - 1]);
    fileReader.readFile(v.getParticles());

    #if 0
    ParticleContainer c;

    for (Particle& p : v.getParticles()) {
        c.addParticle(p);
    }

    for(const auto& p : c) {
        std::cout << p.toString() << "\n";
    }
    #endif

    v.runSimulation();

    return 0;
}