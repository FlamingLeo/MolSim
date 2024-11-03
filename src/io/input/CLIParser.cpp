#include "CLIParser.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <unordered_map>

void CLIParser::checkValidity(const Arguments &args) {
    // maybe disallow start and end being the same? or print out some warning?
    if (args.startTime > args.endTime)
        CLIUtils::error("Start time must be before end time!");
    if (args.delta_t <= 0)
        CLIUtils::error("Timestep must be positive!");
}

void CLIParser::parseArguments(int argc, char **argv, Arguments &args) {
    int ch;                       // store option character
    CLIUtils::filename = argv[0]; // update filename for help and usage strings
    opterr = 0;                   // silence default getopt error messages

    SPDLOG_DEBUG("Parsing command line arguments...");

    // check for invalid syntax (not enough args)
    if (argc < 2)
        CLIUtils::error("Not enough arguments! Use '-h' to display a help message.");

    // loop over all of the options
    while ((ch = getopt(argc, argv, OPTSTRING)) != -1) {
        SPDLOG_TRACE("[getopt] Read option -{} with {}argument {}", StringUtils::fromChar(ch), optarg ? "" : "no ",
                     optarg ? optarg : "" /* this is really tacky but it works */);
        switch (ch) {
        case 's': /* start time */
            args.startTime = StringUtils::toDouble(optarg);
            SPDLOG_DEBUG("Set start time to {}.", args.startTime);
            break;
        case 'e': /* end time */
            args.endTime = StringUtils::toDouble(optarg);
            SPDLOG_DEBUG("Set end time to {}.", args.endTime);
            break;
        case 'd': /* timestep */
            args.delta_t = StringUtils::toDouble(optarg);
            SPDLOG_DEBUG("Set timestep to {}.", args.delta_t);
            break;
        case 'f': /* output frequency */
            args.itFreq = StringUtils::toInt(optarg);
            SPDLOG_DEBUG("Set output frequency to {}.", args.itFreq);
            break;
        case 'o': /* output type */
            args.type = StringUtils::toWriterType(optarg);
            SPDLOG_DEBUG("Set output type to {}.", optarg);
            break;
        case 't': /* simulation type */
            args.sim = StringUtils::toSimulationType(optarg);
            SPDLOG_DEBUG("Set simulation type to {}.", optarg);
            break;
        case 'h': /* help */
            CLIUtils::printHelp();
            std::exit(EXIT_SUCCESS);
        case '?': /* invalid syntax */
            SPDLOG_TRACE("[getopt] Found invalid or incomplete option: {}.", StringUtils::fromChar(optopt));
            if (optopt == 's')
                CLIUtils::error("Start time not specified!");
            else if (optopt == 'e')
                CLIUtils::error("End time not specified!");
            else if (optopt == 'd')
                CLIUtils::error("Timestep not specified!");
            else if (optopt == 'f')
                CLIUtils::error("Output frequency not specified!");
            else if (optopt == 'o')
                CLIUtils::error("Output type not specified!");
            else if (optopt == 't')
                CLIUtils::error("Simulation type not specified!");
            else
                CLIUtils::error("Unknown option found", StringUtils::fromChar(optopt));
        default: /* shouldn't happen... */
            CLIUtils::error_log("An unknown error occurred while parsing command line arguments!");
        }
    }
    SPDLOG_TRACE("[getopt] Finished iterating through options.");

    // handle getopt's special "--" argument
    if (std::strcmp(argv[optind - 1], "--") == 0)
        CLIUtils::error("Invalid option '--'!");

    // by this point, following correct syntax, optind should be exactly one less than the argument counter
    SPDLOG_TRACE("optind: {}, argc: {}", optind, argc);
    if (optind != (argc - 1))
        CLIUtils::error("Invalid syntax - no file input provided!");

    // finally, check numerical argument validity and return arguments if all goes well
    SPDLOG_TRACE("Checking argument validity.");
    checkValidity(args);

    SPDLOG_DEBUG("Finished parsing command line arguments.");
}