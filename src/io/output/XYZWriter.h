/**
 * @file XYZWriter.h
 * @author eckhardw
 * @brief Class used to generate XYZ output.
 * @date 2010-03-01
 *
 * @copyright Copyright (c) 2010 - 2024
 *
 */

#pragma once

#include "objects/Particle.h"
#include "FileWriter.h"
#include <fstream>
#include <list>
#define OUTPUT_DIR "xyz"

namespace outputWriter
{

  /**
   * @brief This class implements the functionality to generate VTK output from particles.
   *
   */
  class XYZWriter : public FileWriter
  {

  public:
    XYZWriter();
    virtual ~XYZWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a list of particles to a XYZ file.
     *
     * @param particles A list of the particles.
     * @param filename The base name of the output file to be written.
     * @param iteration The number of the current iteration, used to generate a unique filename.
     */
    void writeParticles(std::list<Particle> particles, const std::string &filename, int iteration);
  };

} // namespace outputWriter
