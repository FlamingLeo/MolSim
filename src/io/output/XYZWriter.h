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

#include "FileWriter.h"
#include "objects/Particle.h"
#include <fstream>
#include <list>

namespace outputWriter {

/**
 * @brief This class implements the functionality to generate VTK output from
 * particles.
 *
 */
class XYZWriter : public FileWriter {
  private:
    /// @brief The base name of the generated files.
    std::string m_basename = "MD_xyz";
    /// @brief The name of the directory in which to store the generated files.
    std::string m_dirname = "xyz";

  public:
    /// @brief Creates a new uninitialized XYZWriter.
    XYZWriter();

    /**
     * @brief Creates a new XYZWriter with the given basename for future file outputs.
     * 
     * @param basename The base name of the generated files.
     */
    XYZWriter(const std::string &basename);

    /**
     * @brief Creates a new XYZWriter with the given basename for future file outputs in the specified directory.
     * 
     * @param basename The base name of the generated files.
     * @param dirname The directory name of the generated files.
     */
    XYZWriter(const std::string &basename, const std::string &dirname);

    /// @brief Destroys the current XYZWriter object.
    virtual ~XYZWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a list of
     * particles to a XYZ file.
     *
     * @param particles A list of the particles.
     * @param iteration The number of the current iteration, used to generate a
     * unique filename.
     */
    void writeParticles(ParticleContainer &particles, int iteration) override;
};

} // namespace outputWriter
