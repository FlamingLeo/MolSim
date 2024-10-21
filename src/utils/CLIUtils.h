/**
 * @file CLIUtils.h
 * @brief Utility functions for console input / output.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <iostream>
#include <cstdlib>
#include <string_view>

namespace CLIUtils
{
    /**
     * @brief The filename of the executable (default: "./MolSim").
     */
    std::string_view filename{"./MolSim"};

    /**
     * @brief Prints a usage string explaining the syntax of the main program.
     */
    void printUsage()
    {
        std::cerr << "USAGE: " << filename << " [options] <filename>\n";
    }

    /**
     * @brief Prints a help string explaining the functionality of the main program.
     */
    void printHelp()
    {
        std::cout << "The PSE Molecular Dynamics simulation program, developed by Group C.\n\n"
                     "USAGE: "
                  << filename << " [options] <filename>\n"
                                 "OPTIONS:\n"
                                 "-s <number> : Sets the start time (decimal) for a specific simulation (default: 0).\n"
                                 "-e <number> : Sets the end time (decimal) for a specific simulation (default: 1000).\n"
                                 "-d <number> : Sets the time interval between two iterations of a simulation (default: 0.014).\n"
                                 "-h          : Prints out a help message. Doesn't perform any simulation.\n";
    }

    /**
     * @brief Prints an error message to stderr with the prefix "ERROR: ", prints the usage string and exits with EXIT_FAILURE.
     *
     * @param msg The message to be printed to stderr.
     */
    void error(const char *msg)
    {
        std::cerr << "ERROR: " << msg << "\n";
        printUsage();
        std::exit(EXIT_FAILURE);
    }
}
