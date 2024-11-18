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

class XMLReader : public FileReader {
  private:
    std::string m_filename;

  public:
    explicit XMLReader(const std::string &filename);
    void readXML(Arguments &args, ParticleContainer &pc);
};