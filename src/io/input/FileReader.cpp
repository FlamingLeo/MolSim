#include "FileReader.h"
#include "utils/CLIUtils.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

FileReader::FileReader() = default;

FileReader::FileReader(const std::string &filename) { openFile(filename); };

FileReader::~FileReader() = default;

void FileReader::openFile(const std::string &filename) {
    // check if input file is a regular file
    if (!(std::filesystem::is_regular_file(filename)))
        CLIUtils::error("Input file path is not a regular file", filename, true);

    // check if file exists
    if (!(std::filesystem::exists(filename)))
        CLIUtils::error("Input file does not exist", filename, true);

    // try to open file
    m_infile.open(filename);
    if (!(m_infile.is_open()))
        CLIUtils::error("Could not open file", filename);
}

void FileReader::readFile(ParticleContainer *particles) {
    // check if file is open and particles is not null
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!");

    if (!particles) {
        CLIUtils::error("Cannot read into uninitialized ParticleContainer!");
    }

    // parse file content into particle list
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;
    std::string tmp_string;

    getline(m_infile, tmp_string);
    std::cout << "Read line: " << tmp_string << std::endl;

    while (tmp_string.empty() or tmp_string[0] == '#') {
        getline(m_infile, tmp_string);
        std::cout << "Read line: " << tmp_string << std::endl;
    }

    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    std::cout << "Reading " << num_particles << "." << std::endl;
    getline(m_infile, tmp_string);
    std::cout << "Read line: " << tmp_string << std::endl;

    for (int i = 0; i < num_particles; i++) {
        std::istringstream datastream(tmp_string);

        for (auto &xj : x)
            datastream >> xj;
        for (auto &vj : v)
            datastream >> vj;
        if (datastream.eof())
            CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i));
        datastream >> m;
        particles->addParticle(x, v, m);

        getline(m_infile, tmp_string);
        std::cout << "Read line: " << tmp_string << std::endl;
    }
}
