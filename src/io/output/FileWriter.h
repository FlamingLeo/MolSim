/**
 * @file FileWriter.h
 * @brief General class used to create some sort of file output from given input data.
 * @date 2024-10-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <string>

/// @brief The file writer superclass which implements functionality to output data to a file on disk.
class FileWriter
{
public:

    /**
     * @brief Creates / opens a file, and overwrites the contents the file with a given string. 
     * 
     * @param filename The name of the file to be opened.
     * @param content The string to be written inside of the file.
     * @return true if the content was successfully written.
     * @return false if an error occurred while attempting to write the file contents.
     */
    bool writeFile(const std::string &filename, const std::string &content);
};