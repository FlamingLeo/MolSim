/**
 * @file XMLWriter.h
 * @brief Serialize simulation data to an XML file.
 * @version 0.1
 * @date 2024-12-08
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "objects/FlowSimulationAnalyzer.h"
#include "objects/ParticleContainer.h"
#include "objects/Thermostat.h"
#include "utils/Arguments.h"
#include <fstream>
#include <string>

/// @brief Class used to serialize simulation results to an XML file which can once again be used as input.
class XMLWriter {
  private:
    /// @brief Output stream containing the XML file to write contents to.
    std::ofstream m_file;
    /// @brief The name of the output file.
    std::string m_filename;

    /**
     * @brief Opens a new file for writing.
     *
     * @param filename The name of the file to be written to.
     */
    void openFile(const std::string &filename);

    /// @brief Closes the opened file.
    void closeFile();

  public:
    /**
     * @brief Constructs a new XMLWriter object.
     *
     * @param filename The name of the file to be written to.
     */
    explicit XMLWriter(const std::string &filename);

    /// @brief Destroys the current XMLWriter object.
    ~XMLWriter();

    /**
     * @brief Serializes simulation data to an XML file.
     *
     * @param pc The ParticleContainer containing the active simulation particles.
     * @param args The Arguments struct containing simulation metadata.
     * @param t The thermostat.
     * @param fsa The analyzer, which may be on or off. If disabled, it will not be serialized.
     */
    void serialize(const ParticleContainer &pc, const Arguments &args, const Thermostat &t,
                   const FlowSimulationAnalyzer &fsa);
};