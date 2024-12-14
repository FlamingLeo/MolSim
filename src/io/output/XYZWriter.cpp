#include "XYZWriter.h"
#include "utils/CLIUtils.h"
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

XYZWriter::XYZWriter() {
    SPDLOG_TRACE("Created new XYZWriter (empty).");
    initializeFolder(m_dirname);
}
XYZWriter::XYZWriter(const std::string &basename) : m_basename{basename} {
    SPDLOG_TRACE("Created new XYZWriter with base name {}", basename);
    initializeFolder(m_dirname);
}
XYZWriter::XYZWriter(const std::string &basename, const std::string &dirname)
    : m_basename{basename}, m_dirname{dirname} {
    SPDLOG_TRACE("Created new XYZWriter with base name {} and directory name {}", basename, dirname);
    initializeFolder(m_dirname);
}
XYZWriter::~XYZWriter() {
    closeFile();
    SPDLOG_TRACE("Destroyed XYZWriter.");
}

void XYZWriter::writeParticles(const ParticleContainer &particles, int iteration, int total) {
    // define file name
    std::stringstream strstr, content;
    strstr << m_dirname << "/" << m_basename << "_" << std::setfill('0') << std::setw(4) << iteration << ".xyz";

    openFile(strstr.str());

    // write content to preliminary string stream
    content << particles.size() << std::endl;
    content << "Generated by MolSim 2024-25. See "
               "http://openbabel.org/wiki/XYZ_(format) for "
               "file format documentation."
            << std::endl;

    for (auto &p : particles) {
        std::array<double, 3> x = p.getX();
        content << "Ar ";
        content.setf(std::ios_base::showpoint);

        for (auto &xi : x) {
            content << xi << " ";
        }

        content << std::endl;

        SPDLOG_TRACE("Plotted Particle - {}", p.toString());
    }

    // finalize output
    writeFile(content.str(), strstr.str(), iteration, total);
}
