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
#include <string>

/// @brief Class which implements functionality to generate VTK output from particles.
class VTKWriter : public FileWriter {
  private:
    /// @brief The VTK file instance containing particle data.
    VTKFile_t *m_vtkFile;
    /// @brief The base name of the generated files.
    std::string m_basename = "MD_vtk";
    /// @brief The name of the directory in which to store the generated files.
    std::string m_dirname = "vtk";

    /**
     * @brief (Re)initializes the output directory.
     *
     * First, the function checks if the output directory exists. If it doesn't, it creates it. Otherwise, it deletes
     * everything inside it.
     *
     */
    void initializeFolder();

    /**
     * @brief Initializes m_vtkFile with necessary metadata for writing purposes. Terminates program execution on error.
     *
     * @param numParticles The total number of particles.
     */
    void initializeOutput(int numParticles);

    /**
     * @brief Writes the contents of m_vtkFile into a given VTK file. Terminates program execution on error.
     *
     * @param iteration The current iteration of the simulation.
     * @param total The total number of iterations, used to display the current percentage.
     */
    void writeFile(int iteration, int total);

    /**
     * @brief Plots a single particle to m_vtkFile. Terminates program execution on error.
     *
     * @param p The particle to be plotted.
     */
    void plotParticle(const Particle &p);

  public:
    /// @brief Creates a new uninitialized VTKWriter.
    VTKWriter();

    /**
     * @brief Creates a new VTKWriter with the given basename for future file outputs.
     *
     * @param basename The base name of the generated files.
     */
    explicit VTKWriter(const std::string &basename);

    /**
     * @brief Creates a new VTKWriter with the given basename for future file outputs in the specified directory.
     *
     * @param basename The base name of the generated files.
     * @param dirname The directory name of the generated files.
     */
    VTKWriter(const std::string &basename, const std::string &dirname);

    /// @brief Destroys the current VTKWriter object.
    virtual ~VTKWriter();

    /**
     * @brief Writes the type, mass, position, velocity and force of a ParticleContainer to a VTK file. Terminates
     * program execution on error.
     *
     * @param particles The ParticleContainer.
     * @param iteration The number of the current iteration, used to generate a unique filename.
     * @param total The total number of iterations, used to display the current percentage.
     */
    void writeParticles(const ParticleContainer &particles, int iteration, int total) override;
};
