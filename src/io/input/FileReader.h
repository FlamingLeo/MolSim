/**
 * @file FileReader.h
 * @author eckhardw
 * @brief General class used to read and parse file input into some container.
 * @date 2010-02-23
 *
 * @copyright Copyright (c) 2010 - 2024
 *
 */
#pragma once

#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include <fstream>
#include <string>
#include <vector>

/// @brief File reader class which implements functionality to read data from input files.
class FileReader {
  protected:
    /// @brief Input stream representing the file containing molecule data.
    std::ifstream m_infile;

  public:
    /// @brief Creates a new FileReader with no file initialized.
    FileReader();

    /**
     * @brief Creates a new FileReader and opens the file with the given name.
     *
     * @param filename The name of the file to load into m_infile.
     */
    explicit FileReader(const std::string &filename);

    /// @brief Destroys the FileReader object and automatically closes the input stream.
    virtual ~FileReader();

    /**
     * @brief Initializes the input stream with the given file (via its name).
     *
     * @param filename The name of the file to load into the input stream m_infile.
     */
    void openFile(const std::string &filename);
};
