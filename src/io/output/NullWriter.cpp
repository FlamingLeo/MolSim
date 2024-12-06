#include "NullWriter.h"
#include "utils/CLIUtils.h"
#include <spdlog/spdlog.h>
#include <string>

NullWriter::NullWriter() { SPDLOG_TRACE("Created a new NullWriter (empty)."); }
NullWriter::NullWriter(const std::string &) { SPDLOG_TRACE("Created a new NullWriter (basename)."); }
NullWriter::NullWriter(const std::string &, const std::string &) {
    SPDLOG_TRACE("Created a new NullWriter (basename, dirname).");
}
NullWriter::~NullWriter() { SPDLOG_TRACE("Destroyed NullWriter."); }

void NullWriter::writeParticles(const ParticleContainer &particles, int iteration, int total) {
    const int percentage = CLIUtils::getPercentage(iteration, total);
    SPDLOG_INFO("Iteration: {} ({}%)", iteration, percentage);
    for (size_t i = 0; i < particles.size(); ++i)
        SPDLOG_INFO("Particle {}: {}", i, particles[i].toString());
}