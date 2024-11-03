#include "FileWriter.h"
#include "utils/CLIUtils.h"
#include <fstream>
#include <spdlog/spdlog.h>

FileWriter::FileWriter() { SPDLOG_TRACE("Created new FileWriter (empty)."); };
FileWriter::FileWriter(const std::string &filename) {
    openFile(filename);
    SPDLOG_TRACE("Created new FileWriter with file {}.", filename);
}
FileWriter::~FileWriter() {
    closeFile();
    SPDLOG_TRACE("Destroyed FileWriter.");
}

void FileWriter::closeFile() {
    if (m_file.is_open()) {
        m_file.close();
        SPDLOG_DEBUG("Closed file.");
    } else {
        SPDLOG_TRACE("No file to close.");
    }
}

void FileWriter::openFile(const std::string &filename) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename);

    if (!m_file.is_open())
        CLIUtils::error_log("Failed to open file", filename);

    SPDLOG_DEBUG("Opened file {} for writing.", filename);
}

void FileWriter::writeFile(const std::string &content, const std::string &filename = "") {
    if (!(m_file.is_open()))
        CLIUtils::error_log("No file opened for writing!");

    m_file << content;

    if (m_file.bad())
        CLIUtils::error_log("Failed to write contents to file stream!");

    SPDLOG_DEBUG("Wrote contents to file{}.", filename.empty() ? "" : " " + filename);
}
