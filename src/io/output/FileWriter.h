/**
 * @file FileWriter.h
 * @brief General class used to create some sort of file output from given input
 * data.
 * @date 2024-10-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/ParticleContainer.h"
#include <fstream>
#include <string>

/// @brief Superclass which implements functionality to output data to a file on disk.
class FileWriter {
  protected:
    /// @brief Output stream containing the file to write contents to.
    std::ofstream m_file;

  public:
    /// @brief Creates a new FileWriter with no file initialized.
    FileWriter();

    /**
     * @brief Creates a new FileWriter and opens the file with the given name.
     *
     * @param filename The name of the file to load into the output stream m_file.
     */
    explicit FileWriter(const std::string &filename);

    /// @brief Closes the opened file and destroys the FileWriter object.
    virtual ~FileWriter();

    /**
     * @brief Opens / creates the file with the given name.
     *
     * @param filename The name of the file to load into the output stream m_file.
     */
    void openFile(const std::string &filename);

    /// @brief Closes the opened file if it exists, otherwise does nothing.
    void closeFile();

    /**
     * @brief Overwrites the contents of the file opened in m_file with a given string. On error, the program will
     * terminate.
     *
     * @param content The string to be written inside of the file.
     * @param filename An optional filename to be used with spdlog to display the name of the file currently being
     * written to.
     */
    void writeFile(const std::string &content, const std::string &filename = "");

    /**
     * @brief Interface function for writing the type, mass, position, velocity and force of a ParticleContainer to a
     * file. Terminates program execution on error.
     *
     * @param particles The ParticleContainer.
     * @param iteration The number of the current iteration, used to generate a unique filename.
     */
    virtual void writeParticles(const ParticleContainer &particles, int iteration) = 0;
};
