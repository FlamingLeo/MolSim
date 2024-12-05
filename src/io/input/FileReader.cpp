#include "FileReader.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <vector>

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