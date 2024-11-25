#include "NullWriter.h"
#include <spdlog/spdlog.h>
#include <string>

NullWriter::NullWriter() { SPDLOG_TRACE("Created a new NullWriter (empty)."); }
NullWriter::NullWriter(const std::string &) { SPDLOG_TRACE("Created a new NullWriter (basename)."); }
NullWriter::NullWriter(const std::string &, const std::string &) {
    SPDLOG_TRACE("Created a new NullWriter (basename, dirname).");
}
NullWriter::~NullWriter() { SPDLOG_TRACE("Destroyed NullWriter."); }

void NullWriter::writeParticles(const ParticleContainer &particles, int iteration) {
    SPDLOG_INFO("Iteration: {}", iteration);
    for (size_t i = 0; i < particles.size(); ++i)
        SPDLOG_INFO("Particle {}: {}", i, particles[i].toString());
}