// NOTE: someone _please_ tell the LRZ people to update their c++ compilers, thanks.
#include "FileReader.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

FileReader::FileReader() = default;
FileReader::FileReader(const std::string &filename) { openFile(filename); }
FileReader::~FileReader() = default;

void FileReader::openFile(const std::string &filename) {
    // stat buffer for file metadata
    struct stat buffer;

    // check if file exists
    if (stat(filename.c_str(), &buffer) != 0)
        CLIUtils::error("Input file does not exist", filename);

    // check if input file is a regular file
    if (!S_ISREG(buffer.st_mode))
        CLIUtils::error("Input file path is not a regular file; make sure the input is passed last", filename);

    // try to open file
    m_infile.open(filename);
    if (!(m_infile.is_open()))
        CLIUtils::error("Could not open file", filename, false);

    SPDLOG_DEBUG("Opened file {} for reading.", filename);
}