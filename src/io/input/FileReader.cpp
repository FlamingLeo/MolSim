#include "FileReader.h"
#include "utils/CLIUtils.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

FileReader::FileReader() = default;

FileReader::FileReader(const std::string &filename) { openFile(filename); };

FileReader::~FileReader() = default;

void FileReader::openFile(const std::string &filename) {
    // check if input file is a regular file
    if (!(std::filesystem::is_regular_file(filename)))
        CLIUtils::error("Input file path is not a regular file", filename);

    // check if file exists
    if (!(std::filesystem::exists(filename)))
        CLIUtils::error("Input file does not exist", filename);

    // try to open file
    m_infile.open(filename);
    if (!(m_infile.is_open()))
        CLIUtils::error("Could not open file", filename);

    SPDLOG_DEBUG("Opened file {} for reading.", filename);
}

void FileReader::readFile(ParticleContainer *particles) {
    // check if file is open and particles is not null
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!");

    if (!particles) {
        CLIUtils::error("Cannot read into uninitialized ParticleContainer!");
    }

    // parse file content into particle container
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;
    std::string tmp_string;

    getline(m_infile, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    while (tmp_string.empty() or tmp_string[0] == '#') {
        getline(m_infile, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }

    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    SPDLOG_DEBUG("Reading {} Particle objects.", tmp_string);
    getline(m_infile, tmp_string);
    SPDLOG_DEBUG("Read Particle data: {}", tmp_string);

    // reserve space for particles to avoid expensive copying
    particles->reserve(num_particles);

    // populate particle container
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
        SPDLOG_DEBUG("Read {}Particle data{}", tmp_string.empty() ? "no more " : "",
                     tmp_string.empty() ? "." : ": " + tmp_string);
    }

    SPDLOG_TRACE("Finalized ParticleContainer - {}", particles->toString());
}
