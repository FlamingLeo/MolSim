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

#include "objects/Particle.h"
#include "io/vtk/vtk-unstructured.h"
#include "FileWriter.h"
#include <list>
#define OUTPUT_DIR "vtk"

namespace outputWriter
{

  /**
   * @brief This class implements the functionality to generate VTK output from particles.
   *
   */
  class VTKWriter : public FileWriter
  {

  private:
    /// @brief The VTK file instance containing particle data.
    VTKFile_t *m_vtkFile;

    /**
     * @brief Initializes m_vtkFile with necessary metadata for writing purposes.
     * 
     * @param numParticles The total number of particles.
     * @return true if the VTK file was successfully initialized.
     * @return false if the VTK file could not be initialized properly.
     */
    bool initializeOutput(int numParticles);
    
    /**
     * @brief Writes the contents of m_vtkFile into a given VTK file.
     * 
     * @param filename The name of the output file in which to write the VTK data.
     * @param iteration The current iteration of the simulation.
     * @return true if the data was successfully written.
     * @return false if the data could not be written to the output file.
     */
    bool writeFile(const std::string &filename, int iteration);
    
    /**
     * @brief Plots a single particle to m_vtkFile.
     * 
     * @param p The particle to be plotted.
     * @return true if the particle was succesfully plotted.
     * @return false if the particle could not be plotted properly.
     */
    bool plotParticle(const Particle &p);

  public:
    VTKWriter();

    virtual ~VTKWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a list of particles to a VTK file.
     *
     * @param particles A list of the particles.
     * @param filename The base name of the output file to be written.
     * @param iteration The number of the current iteration, used to generate a unique filename.
     */
    void writeParticles(const std::list<Particle> &particles, const std::string &filename, int iteration);
  };

} // namespace outputWriter
