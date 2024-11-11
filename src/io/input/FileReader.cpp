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
#include <vector>

/// @brief Calls getline(...) and checks for errors.
/// @param infile A reference to the input file stream.
/// @param str The buffer to store the file data into.
static void safeGetline(std::ifstream &infile, std::string &str) {
    std::getline(infile, str);
    if (infile.eof()) {
        CLIUtils::error("Malformed input file! Unexpectedly reached EOF.");
    }
    if (infile.fail()) {
        CLIUtils::error("Malformed input file! Data cannot be interpreted as text.");
    }
}

/* documented functions start here */
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

void FileReader::readParticles(ParticleContainer *particles) {
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
    int numParticles = 0;
    std::string tmpString;

    safeGetline(m_infile, tmpString);
    SPDLOG_DEBUG("Read line: {}", tmpString);

    while (tmpString.empty() || tmpString[0] == '#') {
        safeGetline(m_infile, tmpString);
        SPDLOG_DEBUG("Read line: {}", tmpString);
    }

    std::istringstream numstream(tmpString);
    numstream >> numParticles;
    SPDLOG_DEBUG("Reading {} Particle objects.", tmpString);
    std::getline(m_infile, tmpString);
    SPDLOG_DEBUG("Read Particle data: {}", tmpString);

    // reserve space for particles to avoid expensive copying
    particles->reserve(numParticles);

    // populate particle container
    for (size_t i = 0; i < numParticles; i++) {
        std::istringstream datastream(tmpString);

        for (auto &xj : x)
            datastream >> xj;
        for (auto &vj : v)
            datastream >> vj;
        if (datastream.eof())
            CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i), false);
        datastream >> m;
        particles->addParticle(x, v, m);

        std::getline(m_infile, tmpString);
        SPDLOG_DEBUG("Read {}Particle data{}", tmpString.empty() ? "no more " : "",
                     tmpString.empty() ? "." : ": " + tmpString);
    }

    SPDLOG_TRACE("Finalized ParticleContainer - {}", particles->toString());
}

void FileReader::readCuboids(ParticleContainer &particles) {
    // check if file is open and particles is not null
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!", "", false);

    // parse file content into particle container
    struct CuboidData {
        std::array<double, 3> x;
        std::array<double, 3> v;
        std::array<int, 3> N;
        double h;
        double m;
    } c;

    // we use a vector to tally up the total size for reservation
    // we also use a struct to avoid having to use 5 separate vectors...
    std::vector<CuboidData> cs;

    int numParticles = 0;
    size_t particlesSize = particles.size();
    std::string tmpString;

    safeGetline(m_infile, tmpString);
    SPDLOG_DEBUG("Read line: {}", tmpString);

    while (tmpString.empty() || tmpString[0] == '#') {
        safeGetline(m_infile, tmpString);
        SPDLOG_DEBUG("Read line: {}", tmpString);
    }

    std::istringstream numstream(tmpString);
    numstream >> numParticles;
    SPDLOG_DEBUG("Reading {} Cuboid objects.", tmpString);
    std::getline(m_infile, tmpString);
    SPDLOG_DEBUG("Read Cuboid data: {}", tmpString);

    // populate cuboid container
    for (size_t i = 0; i < numParticles; i++) {
        std::istringstream datastream(tmpString);

        for (auto &xj : c.x)
            datastream >> xj;
        for (auto &vj : c.v)
            datastream >> vj;
        for (auto &Nj : c.N)
            datastream >> Nj;
        if (datastream.eof())
            CLIUtils::error("EOF reached unexpectedly reading from line", std::to_string(i), false);
        datastream >> c.h;
        datastream >> c.m;

        SPDLOG_TRACE("Read object data - x : {}, v : {}, N : {}, h : {}, m : {}.", StringUtils::fromArray(c.x),
                     StringUtils::fromArray(c.v), StringUtils::fromArray(c.N), c.h, c.m);

        cs.push_back(c);
        particlesSize += c.N[0] * c.N[1] * c.N[2];

        std::getline(m_infile, tmpString);
        SPDLOG_DEBUG("Read {}Cuboid data{}", tmpString.empty() ? "no more " : "",
                     tmpString.empty() ? "." : ": " + tmpString);
    }

    // reserve enough space for _all_ particles
    particles.reserve(particlesSize);

    // initialize cuboid particles
    for (size_t i = 0; i < cs.size(); ++i) {
        Cuboid C{particles, cs[i].x, cs[i].N, cs[i].v, cs[i].h, cs[i].m};
        C.initializeParticles();
    }
}