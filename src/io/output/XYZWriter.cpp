/*
 * XYZWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "XYZWriter.h"
#include "utils/CLIUtils.h"
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace outputWriter
{

  XYZWriter::XYZWriter() = default;

  XYZWriter::~XYZWriter() = default;

  void XYZWriter::writeParticles(std::list<Particle> particles, const std::string &filename, int iteration)
  {
    // create output directory in which to store generated XYZ output files
    if (!(std::filesystem::exists(OUTPUT_DIR)))
    {
      if (!(std::filesystem::create_directory(OUTPUT_DIR)))
      {
        CLIUtils::error("Error creating XYZ directory!");
      };
    }

    // define file name
    std::stringstream strstr, content;
    strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

    // write content to preliminary string stream
    content << particles.size() << std::endl;
    content << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for "
               "file format doku."
            << std::endl;

    for (auto &p : particles)
    {
      std::array<double, 3> x = p.getX();
      content << "Ar ";
      content.setf(std::ios_base::showpoint);

      for (auto &xi : x)
      {
        content << xi << " ";
      }

      content << std::endl;
    }

    // finalize output
    writeFile(strstr.str(), content.str());
  }

} // namespace outputWriter
