
#include "io/input/FileReader.h"
#include "io/input/CLIParser.h"
#include "simulations/Verlet.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

int main(int argc, char *argv[])
{
  Arguments args;
  CLIParser::parseArguments(argc, argv, args);

  // this will change when more simulations come
  Verlet v{args};

  // TODO move this into ParticleContainer, preferably
  FileReader fileReader;
  if(!(fileReader.readFile(v.getParticles(), argv[argc - 1]))) {
    exit(EXIT_FAILURE);
  };

  v.runSimulation();

  return 0;
}