/**
 * @file CLIUtils.h
 * @brief Utility functions for console input / output.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <cstdlib>
#include <iostream>
#include <string_view>
#define OPTSTRING "s:e:d:f:o:t:h"

namespace CLIUtils {
/**
 * @brief The filename of the executable (default: "./MolSim").
 */
static inline std::string_view filename{"./MolSim"};

/**
 * @brief Prints a usage string explaining the syntax of the main program.
 */
static inline void printUsage() { std::cerr << "USAGE: " << filename << " [options] <filename>\n"; }

/**
 * @brief Prints a help string explaining the functionality of the main program.
 */
static inline void printHelp() {
    std::cout << "The PSE Molecular Dynamics simulation program, developed by Group C.\n\n"
                 "USAGE: "
              << filename
              << " [options] <filename>\n"
                 "OPTIONS:\n"
                 "-s <number>  : Sets the start time (decimal) for a specific simulation (default: 0).\n"
                 "-e <number>  : Sets the end time (decimal) for a specific simulation (default: 1000).\n"
                 "-d <number>  : Sets the time interval between two iterations of a simulation (default: 0.014).\n"
                 "-f <number>  : Sets the output frequency, i.e. after how many iterations a new VTK file should be written (default: 10).\n"
                 "-o [xyz|vtk] : Sets the output file type and directory (default: vtk).\n"
                 "-t [verlet]  : Sets the desired simulation to be performed (default: Verlet) (WIP).\n"
                 "-h           : Prints out a help message. Doesn't perform any simulation.\n";
}

/**
 * @brief Prints an error message to stderr with the prefix "ERROR: ",
 * optionally prints the usage string and exits with EXIT_FAILURE.
 *
 * @param msg The message to be printed to stderr.
 * @param opt An optional extra string to be appended at the end (default:
 * empty)
 * @param usage An optional boolean which defines whether or not the usage
 * string should be printed.
 * @param close An optional boolean which defines whether or not the program
 * should completely exit afterwards.
 */
static inline void error(const char *msg, const std::string &opt = "", bool usage = true, bool close = true) {
    std::cerr << "ERROR: " << msg << (opt.empty() ? "" : ": ") << opt << "\n";
    if (usage)
        printUsage();
    if (close)
        std::exit(EXIT_FAILURE);
}
} // namespace CLIUtils
