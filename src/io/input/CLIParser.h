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
 * @brief Checks if there are enough arguments passed through the command line interface.
 *
 * There must be at least one argument passed (i.e. the file name).
 * Exits the program with an error message if argc is less than 2.
 *
 * @param argc Argument counter, typically passed directly via the main function.
 */
void checkArgc(int argc);

/**
 * @brief Checks if the help option '-h' is passed anywhere.
 *
 * This is done before interpreting the last command line argument as an XML file to enable running the program while
 * only passing the help string in order to print out a help message (i.e. `./MolSim -h` should still be valid syntax,
 * even if no file input is provided).
 *
 * @param argc Argument counter, typically passed directly via the main function.
 * @param argv Argument vector, typically passed directly via the main function.
 */
void checkHelpString(int argc, char **argv);

/**
 * @brief Checks the mathematical validity of the given arguments.
 *
 * @param args A reference to the Argument struct to be checked.
 */
void checkValidity(const Arguments &args);

/**
 * @brief Sets default values for uninitialized Argument attributes based on the chosen Simulation type.
 *
 * @param args A reference to the Argument struct to be modified.
 */
void setDefaults(Arguments &args);

/**
 * @brief Parses and store CLI arguments in an Arguments struct.
 *
 * @param argc Argument counter, typically passed directly via the main function.
 * @param argv Argument vector, typically passed directly via the main function.
 * @param args A reference to the argument struct in which to store the parsed arguments.
 */
void parseArguments(int argc, char **argv, Arguments &args);
}; // namespace CLIParser