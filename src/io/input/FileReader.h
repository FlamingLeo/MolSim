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

#include "objects/Particle.h"
#include <list>

/// @brief The file reader class which implements functionality to read data from input files.
class FileReader
{

public:
  FileReader();
  virtual ~FileReader();

  /**
   * @brief Reads particle data into a list of particles from a given input file.
   *
   * @param particles A list of particles to write the input data into.
   * @param filename The input file containing particle data.
   * @return true if the file contents were succesfully read into the particle list.
   * @return false if the file contents could not be read into the particle list.
   */
  bool readFile(std::list<Particle> &particles, char *filename);
};
