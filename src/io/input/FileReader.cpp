/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"
#include "utils/CLIUtils.h"

#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

FileReader::FileReader() = default;
FileReader::~FileReader() = default;

bool FileReader::readFile(std::list<Particle> &particles, char *filename)
{
  // check if input file is a regular file
  if (!(std::filesystem::is_regular_file(filename)))
  {
    CLIUtils::error("Input file path is not a regular file", filename, true, false);
    return false;
  }

  // check if file exists
  if (!(std::filesystem::exists(filename)))
  {
    CLIUtils::error("Input file does not exist", filename, true, false);
    return false;
  }

  // try to open file
  std::ifstream input_file(filename);
  if (!(input_file.is_open()))
  {
    CLIUtils::error("Could not open file", filename, false, false);
    return false;
  }

  // parse file content into particle list
  std::array<double, 3> x;
  std::array<double, 3> v;
  double m;
  int num_particles = 0;
  std::string tmp_string;

  getline(input_file, tmp_string);
  std::cout << "Read line: " << tmp_string << std::endl;

  while (tmp_string.empty() or tmp_string[0] == '#')
  {
    getline(input_file, tmp_string);
    std::cout << "Read line: " << tmp_string << std::endl;
  }

  std::istringstream numstream(tmp_string);
  numstream >> num_particles;
  std::cout << "Reading " << num_particles << "." << std::endl;
  getline(input_file, tmp_string);
  std::cout << "Read line: " << tmp_string << std::endl;

  for (int i = 0; i < num_particles; i++)
  {
    std::istringstream datastream(tmp_string);

    for (auto &xj : x)
      datastream >> xj;
    for (auto &vj : v)
      datastream >> vj;
    if (datastream.eof())
    {
      CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i), false, false);
      return false;
    }
    datastream >> m;
    particles.emplace_back(x, v, m);

    getline(input_file, tmp_string);
    std::cout << "Read line: " << tmp_string << std::endl;
  }

  return true;
}
