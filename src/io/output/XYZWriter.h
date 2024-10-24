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

namespace outputWriter
{

  /**
   * @brief This class implements the functionality to generate VTK output from particles.
   *
   */
  class XYZWriter : public FileWriter
  {
  private:
    std::string m_basename = "MD_xyz";
    std::string m_dirname = "xyz";

  public:
    XYZWriter();
    XYZWriter(const std::string &basename);
    XYZWriter(const std::string &basename, const std::string &dirname);
    virtual ~XYZWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a list of particles to a XYZ file.
     *
     * @param particles A list of the particles.
     * @param iteration The number of the current iteration, used to generate a unique filename.
     */
    void writeParticles(std::list<Particle> particles, int iteration);
  };

} // namespace outputWriter
