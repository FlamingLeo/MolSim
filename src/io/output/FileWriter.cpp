#include "FileWriter.h"
#include "utils/CLIUtils.h"
#include <fstream>

FileWriter::FileWriter() = default;

FileWriter::FileWriter(const std::string &filename) { openFile(filename); }

FileWriter::~FileWriter() { closeFile(); }

void FileWriter::closeFile() {
    if (m_file.is_open())
        m_file.close();
}

void FileWriter::openFile(const std::string &filename) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename);

    if (!m_file.is_open())
        CLIUtils::error("Failed to open file", filename);
}

void FileWriter::writeFile(const std::string &content) {
    if (!(m_file.is_open()))
        CLIUtils::error("No file opened for writing!");

    m_file << content;
    if (m_file.bad())
        CLIUtils::error("Failed to write contents to file stream!");
}
