#include "VTKWriter.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

VTKWriter::VTKWriter() { SPDLOG_TRACE("Created new VTKWriter (empty)."); }
VTKWriter::VTKWriter(const std::string &basename) {
    (void)basename;
    SPDLOG_TRACE("Created new VTKWriter with base name {}", basename);
}
VTKWriter::VTKWriter(const std::string &basename, const std::string &dirname) {
    (void)basename, (void)dirname;
    SPDLOG_TRACE("Created new VTKWriter with base name {} and directory name {}", basename, dirname);
}
VTKWriter::~VTKWriter() { SPDLOG_TRACE("Destroyed VTKWriter."); }

void VTKWriter::initializeOutput(int numParticles) { (void)numParticles; }

void VTKWriter::writeFile(int iteration, int total) { (void)iteration, (void)total; }

void VTKWriter::plotParticle(const Particle &p) { (void)p; }

void VTKWriter::writeParticles(const ParticleContainer &particles, int iteration, int total) {
    (void)particles, (void)iteration, (void)total;
}
