
#include "io/input/FileReader.h"
#include "io/output/VTKWriter.h"
#include "io/input/CLIParser.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX(double delta_t);

/**
 * calculate the position for all particles
 */
void calculateV(double delta_t);

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argv[])
{
  Arguments args;
  CLIParser::parseArguments(argc, argv, args);

  // TODO check for invalid files (e.g. directories)
  FileReader fileReader;
  fileReader.readFile(particles, argv[argc - 1]);

  double current_time = args.start_time;

  int iteration = 0;

  // for this loop, we assume: current x, current f and current v are known
  while (current_time < args.end_time)
  {
    // calculate new x
    calculateX(args.delta_t);
    // calculate new f
    calculateF();
    // calculate new v
    calculateV(args.delta_t);

    iteration++;
    if (iteration % 10 == 0)
    {
      plotParticles(iteration);
    }
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += args.delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}

void calculateF()
{
  std::list<Particle>::iterator iterator;
  iterator = particles.begin();

  for (auto &p1 : particles)
  {
    p1.setOldF(p1.getF());
    p1.setFToZero();
    for (auto &p2 : particles)
    {
      // @TODO: insert calculation of forces here!
      // where i index is p1 and j index is p2
      if (not(p1 == p2))
      {
        p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                p1.getM() * p2.getM() / std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3), p2.getX() - p1.getX(),
                                std::multiplies<>()));
      }
    }
  }
}

void calculateX(double delta_t)
{
  for (auto &p : particles)
  {
    // @TODO: insert calculation of position updates here!
    p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
           delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
  }
}

void calculateV(double delta_t)
{
  for (auto &p : particles)
  {
    // @TODO: insert calculation of veclocity updates here!
    p.setV(p.getV() + ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>()));
  }
}

void plotParticles(int iteration)
{

  std::string out_name("MD_vtk");

  outputWriter::VTKWriter writer;
  writer.initializeOutput(particles.size());

  for (auto &p : particles)
  {
    writer.plotParticle(p);
  }

  writer.writeFile(out_name, iteration);
}
