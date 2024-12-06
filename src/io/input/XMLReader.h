/**
 * @file XMLReader.h
 * @brief Class used to read and parse XML input data.
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "FileReader.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <string>

/// @brief Class used to read and parse XML input data.
class XMLReader : public FileReader {
  private:
    /// @brief The name of the XML input file. Must be a valid XML file.
    std::string m_filename;

  public:
    /**
     * @brief Construct a new XMLReader object.
     *
     * @param filename The name of the XML input file. Must be a valid XML file.
     */
    explicit XMLReader(const std::string &filename);

    /**
     * @brief Read and parse the contents of a valid XML file into the given Arguments and ParticleContainer references.
     *
     * On error, the program will terminate.
     *
     * @param args A reference to the Arguments struct which will be modified.
     * @param pc A reference to the ParticleContainer object which will be modified.
     */
    void readXML(Arguments &args, ParticleContainer &pc);
};