/**
 * @file NullWriter.h
 * @brief Class used for testing purposes.
 * @date 2024-11-03
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "FileWriter.h"
#include "objects/ParticleContainer.h"
#include <string>

/// @brief Class which doesn't create any file output. To be used only for testing purposes.
class NullWriter : public FileWriter {
  public:
    /// @brief Creates an "uninitialized" NullWriter object.
    NullWriter();

    /**
     * @brief Creates a NullWriter object with a useless base name.
     *
     */
    NullWriter(const std::string &);

    /**
     * @brief Creates a NullWriter object with useless base and directory names.
     *
     */
    NullWriter(const std::string &, const std::string &);

    /// @brief Destroys the NullWriter object.
    virtual ~NullWriter();

    /**
     * @brief Logs the type, mass, position, velocity and force of a ParticleContainer.
     *
     * For debugging purposes only.
     *
     * @param particles The ParticleContainer.
     * @param iteration The number of the current iteration, used to generate a unique log output.
     */
    void writeParticles(const ParticleContainer &particles, int iteration) override;
};