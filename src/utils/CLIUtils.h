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
#include <string>
#include <string_view>
#include <unordered_map>
#define OPTSTRING "s:e:d:f:g:b:o:p:t:B:D:R:h"
#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[0m"
#define RED "\e[0;31m"
#define YEL "\e[0;33m"
#define GRN "\e[0;32m"
#define RST "\e[0m"

/// @brief Namespace defining utility functions for handling console output.
namespace CLIUtils {
/**
 * @brief The filename of the executable (default: "./MolSim").
 */
static inline std::string_view filename{"./MolSim"};

/**
 * @brief Mapping from getopt option characters to their full names.
 */
static inline std::unordered_map<char, std::string> optionNames = {
    {'s', "Start time"},       {'e', "End time"},
    {'d', "Timestep"},         {'b', "Basename"},
    {'f', "Output frequency"}, {'g', "Gravity"},
    {'o', "Output type"},      {'p', "Parallelization type"},
    {'t', "Simulation type"},  {'B', "Boundary Conditions"},
    {'D', "Domain Size"},      {'R', "Cutoff Radius"}};

/**
 * @brief Gets the name of the compiler used to build the program executable (or "unknown" if the compiler is not
 * identifiable via macros).
 *
 * @return The name of the compiler used to build the executable or "unknown".
 */
static inline std::string getCompilerName() {
#if defined(__INTEL_LLVM_COMPILER)
    return "Intel C++ Compiler " + std::to_string(__VERSION);
#elif defined(__clang__)
    return "Clang " + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__) + "." +
           std::to_string(__clang_patchlevel__);
#elif defined(__GNUC__) || defined(__GNUG__)
    return "GNU " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." +
           std::to_string(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    return "Microsoft Visual C++ " + std::to_string(_MSC_VER);
#else
    return "Unknown";
#endif
}

/**
 * @brief Gets the build date and time of the built executable as a string.
 *
 * @return The build date and time of the built executable.
 */
static inline std::string getBuildDate() { return std::string(__DATE__) + " " + __TIME__; }

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
    std::cout
        << "The PSE Molecular Dynamics simulation program, developed by Group C.\n"
        << BOLD_ON << "USAGE" << BOLD_OFF << ": " << filename << " [options] <filename>\n\n"
        << BOLD_ON << "OPTIONS" << BOLD_OFF
        << ":\n"
           "-s <number>  : Sets the start time (decimal) for a specific simulation (default: simulation-specific).\n"
           "-e <number>  : Sets the end time (decimal) for a specific simulation (default: simulation-specific).\n"
           "-d <number>  : Sets the time interval between two iterations of a simulation (default: "
           "simulation-specific).\n"
           "-g <number>  : Sets the gravitational force (decimal) effective on each particle in the simulation "
           "(default: 0.0).\n"
           "-b <name>    : Sets the base name of the generated files (default: type-specific).\n"
           "-B <cccccc>  : Sets the conditions to be applied at each boundary (North, South, West, East, Above, "
           "Below). c is one of:\n"
           "  - o        : Outflow (particles get deleted once they leave the domain).\n"
           "  - r        : Reflective (particles are reflected off the domain boundaries).\n"
           "-D <x,y,z>   : Sets the domain size (decimal array) for the linked cell method (MUST be specified if not "
           "present in input!).\n"
           "-R <number>  : Sets the cutoff radius (decimal) for the linked cell method (MUST be specified if not "
           "present in input!).\n"
           "-f <number>  : Sets the output frequency, i.e. after how many iterations a new VTK file should be "
           "written (default: 10).\n"
           "-o <type>    : Sets the output file type and directory (default: vtk).\n"
           "  - vtk      : Generates VTK Unstructured Grid (.vtu) files.\n"
           "  - xyz      : Generates XYZ (.xyz) files.\n"
           "  - nil      : Logs to stdout. Used for debugging purposes.\n"
           "-p <type>    : Sets the parallelization strategy used (default: coarse).\n"
           "               If OpenMP support is disabled, this option has no effect.\n"
           "  - coarse   : Uses the standard OpenMP for-loop parallelization strategy.\n"
           "  - fine     : Uses a finer-grained, task-based parallelization approach.\n"
           "-t <type>    : Sets the desired simulation to be performed (default: lj).\n"
           "  - gravity  : Performs a gravitational simulation (t_0 = 0, t_end = 1000, dt = 0.014).\n"
           "  - lj       : Performs a simulation of Lennard-Jones potential (t_0 = 0, t_end = 5, dt = 0.0002).\n"
           "-h           : Prints out a help message. Doesn't perform any simulation.\n\n"
        << BOLD_ON << "NOTES" << BOLD_OFF
        << ":\n"
           "Logging must be configured at compile time. To change the log level, read the documentation and "
           "recompile the program accordingly.\n"
           "When specifying the domain size, do NOT use whitespaces between the commas and numbers.\n\n"
        << BOLD_ON << "BUILD DATE" << BOLD_OFF << ": " << getBuildDate() << "\n"
        << BOLD_ON << "COMPILED WITH" << BOLD_OFF << ": " << getCompilerName() << "\n";
}

/**
 * @brief Gets the percentage as an integer from a fraction x / y.
 *
 * @param x The numerator.
 * @param y The denominator. If this is 0, 100 is returned.
 * @return The percentage as a truncated integer.
 */
static inline int getPercentage(int x, int y) {
    return static_cast<int>((y != 0 ? static_cast<double>(x) / y : 1.0) * 100.0);
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
