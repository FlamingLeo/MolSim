#include "FileReader.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

FileReader::FileReader() = default;

FileReader::FileReader(const std::string &filename) { openFile(filename); }

FileReader::~FileReader() = default;

void FileReader::openFile(const std::string &filename) {
    // check if input file is a regular file
    if (!(std::filesystem::is_regular_file(filename)))
        CLIUtils::error("Input file path is not a regular file; make sure the input is passed last", filename);

    // check if file exists
    if (!(std::filesystem::exists(filename)))
        CLIUtils::error("Input file does not exist", filename);

    // try to open file
    m_infile.open(filename);
    if (!(m_infile.is_open()))
        CLIUtils::error("Could not open file", filename, false);

    SPDLOG_DEBUG("Opened file {} for reading.", filename);
}

void FileReader::readFile(ParticleContainer *particles) {
    // check if file is open and particles is not null
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!", "", false);

    if (!particles) {
        CLIUtils::error("Cannot read into uninitialized ParticleContainer!", "", false);
    }

    // parse file content into particle container
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;
    std::string tmp_string;

    getline(m_infile, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    while (tmp_string.empty() || tmp_string[0] == '#') {
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
            CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i), false);
        datastream >> m;
        particles->addParticle(x, v, m);

        getline(m_infile, tmp_string);
        SPDLOG_DEBUG("Read {}Particle data{}", tmp_string.empty() ? "no more " : "",
                     tmp_string.empty() ? "." : ": " + tmp_string);
    }

    SPDLOG_TRACE("Finalized ParticleContainer - {}", particles->toString());
}

void FileReader::readFile(std::vector<Cuboid> &cuboids, ParticleContainer &particles) {
    // parse file content into cuboid container
    std::array<double, 3> x;
    std::array<double, 3> v;
    std::array<int, 3> N;
    double h;
    double m;
    int num_particles = 0;
    std::string tmp_string;

    getline(m_infile, tmp_string);
    SPDLOG_DEBUG("Read line: {}", tmp_string);

    while (tmp_string.empty() || tmp_string[0] == '#') {
        getline(m_infile, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);
    }

    std::istringstream numstream(tmp_string);
    numstream >> num_particles;
    SPDLOG_DEBUG("Reading {} Cuboid objects.", tmp_string);
    getline(m_infile, tmp_string);
    SPDLOG_DEBUG("Read Cuboid data: {}", tmp_string);

    // reserve space for cuboids to avoid expensive copying
    cuboids.reserve(num_particles);

    // populate cuboid container
    for (int i = 0; i < num_particles; i++) {
        std::istringstream datastream(tmp_string);

        for (auto &xj : x)
            datastream >> xj;
        for (auto &vj : v)
            datastream >> vj;
        for (auto &Nj : N)
            datastream >> Nj;
        if (datastream.eof())
            CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i), false);
        datastream >> h;
        datastream >> m;

        particles.reserve(particles.size() + N[0] * N[1] * N[2]);
        Cuboid c{particles, x, N, v, h, m};
        c.initializeParticles();
        cuboids.push_back(c);

        getline(m_infile, tmp_string);
        SPDLOG_DEBUG("Read {}Cuboid data{}", tmp_string.empty() ? "no more " : "",
                     tmp_string.empty() ? "." : ": " + tmp_string);
    }
}