/**
 * @file CLIParser.h
 * @brief Command line argument parser.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <utils/Arguments.h>
#define OPTSTRING "s:e:d:h"

namespace CLIParser
{
    /**
     * @brief Check the mathematical validity of the given arguments.
     *
     * @param args A reference to the argument struct to be checked.
     */
    void checkValidity(const Arguments &args);

    /**
     * @brief Parse and store CLI arguments.
     *
     * @param argc Argument counter, typically passed directly via the main function.
     * @param argv Argument vector, typically passed directly via the main function.
     * @param args A reference to the argument struct to be modified.
     */
    void parseArguments(int argc, char **argv, Arguments &args);
};