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
#include <spdlog/spdlog.h>
#include <string_view>
#define OPTSTRING "s:e:d:f:o:t:h"
#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"

/// @brief Namespace defining utility functions for handling console output.
namespace CLIUtils {
/**
 * @brief The filename of the executable (default: "./MolSim").
 */
static inline std::string_view filename{"./MolSim"};

/**
 * @brief Prints a usage string explaining the syntax of the main program.
 */
static inline void printUsage() {
    std::cerr << "[" << BOLD_ON << "usage" << BOLD_OFF << "] " << filename << " [options] <filename>\n";
}

/**
 * @brief Prints a help string explaining the functionality of the main program.
 */
static inline void printHelp() {
    std::cout << "The PSE Molecular Dynamics simulation program, developed by Group C.\n"
              << BOLD_ON << "USAGE" << BOLD_OFF << ": " << filename << " [options] <filename>\n\n"
              << BOLD_ON << "OPTIONS" << BOLD_OFF
              << ":\n"
                 "-s <number>  : Sets the start time (decimal) for a specific simulation (default: 0).\n"
                 "-e <number>  : Sets the end time (decimal) for a specific simulation (default: 1000).\n"
                 "-d <number>  : Sets the time interval between two iterations of a simulation (default: 0.014).\n"
                 "-f <number>  : Sets the output frequency, i.e. after how many iterations a new VTK file should be "
                 "written (default: 10).\n"
                 "-o <xyz|vtk> : Sets the output file type and directory (default: vtk).\n"
                 "-t <verlet>  : Sets the desired simulation to be performed (default: Verlet) (WIP).\n"
                 "-h           : Prints out a help message. Doesn't perform any simulation.\n\n"
              << BOLD_ON << "NOTES" << BOLD_OFF
              << ":\n"
                 "Logging must be configured at compile time. To change the log level, read the documentation and "
                 "recompile the program accordingly.\n";
}

/**
 * @brief Prints an error message to stderr with the prefix "ERROR: ",
 * optionally prints the usage string and exits with EXIT_FAILURE.
 *
 * This is typically used when the user does something wrong and has a different format to error logs printed with
 * spdlog.
 *
 * @param msg The message to be printed to stderr.
 * @param opt An optional extra string to be appended at the end (default: empty).
 * @param usage An optional boolean which defines whether or not the usage string should be printed.
 * @param close An optional boolean which defines whether or not the program should completely exit afterwards.
 */
static inline void error(const char *msg, const std::string &opt = "", bool usage = true, bool close = true) {
    SPDLOG_ERROR("{}{}{}", msg, (opt.empty() ? "" : ": "), opt);
    if (usage)
        printUsage();
    if (close)
        std::exit(EXIT_FAILURE);
}
} // namespace CLIUtils
