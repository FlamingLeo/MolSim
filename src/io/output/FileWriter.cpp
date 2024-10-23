#include "FileWriter.h"
#include "utils/CLIUtils.h"
#include <fstream>

bool FileWriter::writeFile(const std::string &filename, const std::string &content)
{
    // open the file
    std::ofstream file(filename);
    if (!file)
    {
        CLIUtils::error("Failed to open file", filename, false, false);
        return false;
    }

    // write the string to the file
    file << content;
    if (!file)
    {
        CLIUtils::error("Failed to write contents to file", filename, false, false);
        return false;
    }

    // close the file stream
    file.close();
    return true;
}