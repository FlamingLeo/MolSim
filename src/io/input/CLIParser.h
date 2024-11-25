/**
 * @file CLIParser.h
 * @brief Command line argument parsing functions.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "utils/Arguments.h"

/// @brief Namespace defining functions for parsing command line arguments.
namespace CLIParser {
/**
 * @brief Check the mathematical validity of the given arguments.
 *
 * @param args A reference to the Argument struct to be checked.
 */
void checkValidity(const Arguments &args);

/**
 * @brief Set default values for uninitialized Argument attributes based on the chosen Simulation type.
 *
 * @param args A reference to the Argument struct to be modified.
 */
void setDefaults(Arguments &args);

/**
 * @brief Parse and store CLI arguments in an Arguments struct.
 *
 * @param argc Argument counter, typically passed directly via the main function.
 * @param argv Argument vector, typically passed directly via the main function.
 * @param args A reference to the argument struct in which to store the parsed arguments.
 */
void parseArguments(int argc, char **argv, Arguments &args);
}; // namespace CLIParser