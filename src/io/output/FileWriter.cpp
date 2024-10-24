#include "FileWriter.h"
#include "utils/CLIUtils.h"
#include <fstream>

void FileWriter::writeFile(const std::string &filename, const std::string &content)
{
    // open the file
    std::ofstream file(filename);
    if (!file)
        CLIUtils::error("Failed to open file", filename);

    // write the string to the file
    file << content;
    if (!file)
        CLIUtils::error("Failed to write contents to file", filename);

    // close the file stream
    file.close();
}