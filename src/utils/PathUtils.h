/**
 * @file PathUtils.h
 * @brief Platform-independent utility functions for working with input test files.
 * @date 2024-11-29
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <algorithm>
#include <cstdlib>
#include <limits.h>
#include <string>
#include <unistd.h>

/// @brief  Namespace defining utility functions for working with file paths.
namespace PathUtils {
/**
 * @brief Checks if a file is an XML file by verifying its extension in a case-insensitive way.
 *
 * @param filename The name of (path to) the file to be checked.
 * @return true if the file has a .xml extension.
 * @return false if the file does not have a .xml extension.
 */
static inline bool isXmlFile(const std::string &filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == filename.length() - 1) {
        return false;
    }
    std::string extension = filename.substr(dotPos);
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return extension == ".xml";
}

/**
 * @brief Helper method to get the absolute path of a file or directory contained within a given path.
 *
 * This is used to get the full directory of the project root in order to properly find and address input files,
 * assuming the project directory name is "MolSim".
 *
 * @param fullPath The full path which contains the subpath to be found (haystack).
 * @param targetDir The file or directory name to get the absolute path from (needle).
 * @return The absolute path of the subpath if it can be extracted, or an empty string otherwise.
 */
static inline std::string getTargetPath(const std::string &fullPath, const std::string &targetDir) {
    size_t pos = fullPath.rfind(targetDir);
    if (pos != std::string::npos) {
        return fullPath.substr(0, pos + targetDir.length());
    }
    return "";
}

/**
 * @brief Checks if the test file directory can be found.
 *
 * @param targetPath Out parameter. Here, the absolute path to the test file directory may be stored if the files are
 * found.
 * @param skipReason Out parameter. If something fails, the reason will be logged here.
 * @return true if the test file directory could be found.
 * @return false if the test file directory could not be found.
 */
static inline bool setupFileTests(std::string &targetPath, std::string &skipReason) {
    auto getCurrentPath = []() -> std::string {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            return std::string(buffer);
        }
        return "";
    };
    auto pathExists = [](const std::string &path) -> bool { return access(path.c_str(), F_OK) == 0; };
    std::string cwd = getCurrentPath();
    if (cwd.empty()) {
        skipReason = "Unable to determine current working directory, skipping tests...";
        return false;
    }
    targetPath = PathUtils::getTargetPath(cwd, "MolSim");
    if (targetPath.empty()) {
        skipReason = "Project root directory not found, skipping tests...";
        return false;
    }
    targetPath += "/tests/files";
    if (!pathExists(targetPath)) {
        skipReason = "Test files not found, skipping tests...";
        return false;
    }
    return true;
}
} // namespace PathUtils