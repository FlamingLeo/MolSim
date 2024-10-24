/**
 * @file VTKWriter.h
 * @author eckhardw
 * @brief Class used to generate VTK output.
 * @date 2010-03-01
 *
 * @copyright Copyright (c) 2010 - 2024
 *
 */

#pragma once

#include "FileWriter.h"
#include "io/vtk/vtk-unstructured.h"
#include "objects/Particle.h"
#include <list>

namespace outputWriter {

/**
 * @brief This class implements the functionality to generate VTK output from
 * particles.
 *
 */
class VTKWriter : public FileWriter {

  private:
    /// @brief The VTK file instance containing particle data.
    VTKFile_t *m_vtkFile;
    std::string m_basename = "MD_vtk";
    std::string m_dirname = "vtk";

    /**
     * @brief Initializes m_vtkFile with necessary metadata for writing
     * purposes. Terminates program execution on error.
     *
     * @param numParticles The total number of particles.
     */
    void initializeOutput(int numParticles);

    /**
     * @brief Writes the contents of m_vtkFile into a given VTK file.
     * Terminates program execution on error.
     *
     * @param iteration The current iteration of the simulation.
     */
    void writeFile(int iteration);

    /**
     * @brief Plots a single particle to m_vtkFile.
     * Terminates program execution on error.
     *
     * @param p The particle to be plotted.
     */
    void plotParticle(const Particle &p);

  public:
    VTKWriter();
    VTKWriter(const std::string &basename);
    VTKWriter(const std::string &basename, const std::string &dirname);

    virtual ~VTKWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a list of
     * particles to a VTK file. Terminates program execution on error.
     *
     * @param particles A list of the particles.
     * @param iteration The number of the current iteration, used to generate a
     * unique filename.
     */
    void writeParticles(const std::list<Particle> &particles, int iteration);
};

} // namespace outputWriter
