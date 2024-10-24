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
#include <fstream>

/// @brief Superclass which implements functionality to output data to a file on disk.
class FileWriter
{
protected:
    /// @brief Output stream containing the file to write contents to.
    std::ofstream m_file;

public:
    FileWriter();
    FileWriter(const std::string &filename);
    ~FileWriter();
    
    void openFile(const std::string &filename);
    void closeFile();

    /**
     * @brief Overwrites the contents of the file opened in m_file with a given string.
     *
     * On error, the program will terminate.
     *
     * @param content The string to be written inside of the file.
     */
    void writeFile(const std::string &content);
};