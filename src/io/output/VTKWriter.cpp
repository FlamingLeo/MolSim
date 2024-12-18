#include "VTKWriter.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

VTKWriter::VTKWriter() {
    SPDLOG_TRACE("Created new VTKWriter (empty).");
    initializeFolder(m_dirname);
}
VTKWriter::VTKWriter(const std::string &basename) : m_basename{basename} {
    SPDLOG_TRACE("Created new VTKWriter with base name {}", basename);
    initializeFolder(m_dirname);
}
VTKWriter::VTKWriter(const std::string &basename, const std::string &dirname)
    : m_basename{basename}, m_dirname{dirname} {
    SPDLOG_TRACE("Created new VTKWriter with base name {} and directory name {}", basename, dirname);
    initializeFolder(m_dirname);
}
VTKWriter::~VTKWriter() { SPDLOG_TRACE("Destroyed VTKWriter."); }

void VTKWriter::initializeOutput(int numParticles) {
    return;
}

void VTKWriter::writeFile(int iteration, int total) {
   return;
}

void VTKWriter::plotParticle(const Particle &p) {
   return;
}

void VTKWriter::writeParticles(const ParticleContainer &particles, int iteration, int total) {
    return;
}
