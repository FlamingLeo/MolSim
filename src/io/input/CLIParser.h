/**
 * @file CLIParser.h
 * @brief Command line argument parser.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#define OPTSTRING "s:e:d:h"

/**
 * @brief Struct containing each option configurable via command line arguments.
 *
 */
struct Arguments
{
    /// @brief Start time of a simulation (default: 0).
    double start_time{0};
    /// @brief End time of a simulation (default: 1000).
    double end_time{1000};
    /// @brief Duration of a timestep (default: 0.014).
    double delta_t{0.014};
};

class CLIParser
{
private:
    Arguments args;

public:
    /**
     * @brief Construct a new CLIParser object.
     *
     * @param progname A string used to update the program name inside CLIUtils.
     */
    CLIParser(const char *progname /* = "./MolSim" */);

    /**
     * @brief Check the mathematical validity of the given arguments.
     */
    void checkValidity();

    /**
     * @brief Parse CLI arguments, store them in the args variable and return it.
     *
     * @param argc Argument counter, typically passed directly via the main function.
     * @param argv Argument vector, typically passed directly via the main function.
     * 
     * @return Arguments 
     */
    Arguments parseArguments(int argc, char **argv);

    /**
     * @brief Get CLI arguments.
     *
     * @return Arguments
     */
    Arguments getArgs();
};