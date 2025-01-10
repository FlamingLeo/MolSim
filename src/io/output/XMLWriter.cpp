#include "XMLWriter.h"
#include "utils/CLIUtils.h"
#include "utils/CellUtils.h"
#include "utils/StringUtils.h"
#include <spdlog/spdlog.h>
#include <string>

XMLWriter::XMLWriter(const std::string &filename) : m_filename{filename} {
    openFile(filename);
    SPDLOG_TRACE("Created new XMLWriter with file {}.", filename);
}
XMLWriter::~XMLWriter() {
    closeFile();
    SPDLOG_TRACE("Destroyed XMLWriter.");
}
void XMLWriter::closeFile() {
    if (m_file.is_open()) {
        m_file.close();
        SPDLOG_DEBUG("Closed file.");
    } else {
        SPDLOG_TRACE("No file to close.");
    }
}
void XMLWriter::openFile(const std::string &filename) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename);

    if (!m_file.is_open())
        CLIUtils::error("Failed to open file", filename, "", false);

    SPDLOG_DEBUG("Opened file {} for writing.", filename);
}
void XMLWriter::serialize(const ParticleContainer &pc, const Arguments &args, const Thermostat &t) {
    return; /* REMOVED FOR ICPX COMPATIBILITY */
}
