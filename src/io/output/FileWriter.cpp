#include "FileWriter.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

FileWriter::FileWriter() { SPDLOG_TRACE("Created new FileWriter (empty)."); }
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
        CLIUtils::error("Failed to open file", filename, "", false);

    SPDLOG_DEBUG("Opened file {} for writing.", filename);
}

void FileWriter::writeFile(const std::string &content, const std::string &filename, int iteration, int total) {
    if (!(m_file.is_open()))
        CLIUtils::error("No file opened for writing!", "", false);

    m_file << content;

    if (m_file.bad())
        CLIUtils::error("Failed to write contents to file stream!", "", false);

    SPDLOG_INFO("Wrote contents to file{}{}.", filename.empty() ? "" : " " + filename,
                (iteration == -1 || total == -1)
                    ? ""
                    : (" (" + StringUtils::fromNumber(iteration) + " / " + StringUtils::fromNumber(total) + ") (" +
                       StringUtils::fromNumber(CLIUtils::getPercentage(iteration, total)) + "%)"));
}

void FileWriter::initializeFolder(const std::string &dirname) {
    // stat buffer for folder metadata
    struct stat buffer;

    // check if folder exists and reinitialize
    if (!(stat(dirname.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode))) {
        SPDLOG_DEBUG("Folder {} does not exist, creating...", dirname);
        if (mkdir(dirname.c_str(), 0755) != 0) {
            CLIUtils::error("Error creating directory", dirname, false);
        }
    } else {
        SPDLOG_DEBUG("Folder {} exists, reinitializing...", dirname);
        const std::string command = "rm -rf " + dirname; // kinda brutal but much shorter than the usual C method
        if (system(command.c_str()) != 0) {
            CLIUtils::error("Error removing directory", dirname, false);
        }
        if (mkdir(dirname.c_str(), 0755) != 0) {
            CLIUtils::error("Error reinitializing directory", dirname, false);
        }
    }
}