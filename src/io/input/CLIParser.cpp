#include "CLIParser.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

void CLIParser::checkValidity(const Arguments &args) {
    SPDLOG_TRACE("Checking argument validity...");
    // maybe disallow start and end being the same? or print out some warning?
    if (args.startTime > args.endTime)
        CLIUtils::error("Start time must be before end time!");
    if (args.delta_t <= 0)
        CLIUtils::error("Timestep must be positive!");
    if (args.itFreq <= 0) {
        CLIUtils::error("Output frequency must be positive!");
    }
}

void CLIParser::setDefaults(Arguments &args) {
    SPDLOG_TRACE("Argument bit vector: {}", args.argsSet.to_string());
    switch (args.sim) {
    case SimulationType::VERLET:
        args.startTime = args.argsSet.test(0) ? args.startTime : 0.0;
        args.endTime = args.argsSet.test(1) ? args.endTime : 1000.0;
        args.delta_t = args.argsSet.test(2) ? args.delta_t : 0.014;
        break;
    case SimulationType::LJ:
        args.startTime = args.argsSet.test(0) ? args.startTime : 0.0;
        args.endTime = args.argsSet.test(1) ? args.endTime : 5.0;
        args.delta_t = args.argsSet.test(2) ? args.delta_t : 0.0002;
        break;
    default:
        CLIUtils::error("Cannot set default arguments for unknown simulation type!");
    }
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
            args.argsSet.set(0);
            SPDLOG_DEBUG("Set start time to {}.", args.startTime);
            break;
        case 'e': /* end time */
            args.endTime = StringUtils::toDouble(optarg);
            args.argsSet.set(1);
            SPDLOG_DEBUG("Set end time to {}.", args.endTime);
            break;
        case 'd': /* timestep */
            args.delta_t = StringUtils::toDouble(optarg);
            args.argsSet.set(2);
            SPDLOG_DEBUG("Set timestep to {}.", args.delta_t);
            break;
        case 'E': /* epsilon */
            args.epsilon = StringUtils::toDouble(optarg);
            SPDLOG_DEBUG("Set epsilon to {}.", args.epsilon);
            break;
        case 'S': /* sigma */
            args.sigma = StringUtils::toDouble(optarg);
            SPDLOG_DEBUG("Set sigma to {}.", args.sigma);
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
        {
            SPDLOG_TRACE("[getopt] Found invalid or incomplete option: {}.", StringUtils::fromChar(optopt));
            auto it = CLIUtils::optionNames.find(optopt);
            if (it != CLIUtils::optionNames.end()) {
                std::string err = it->second + " not specified!";
                CLIUtils::error(err.c_str());
            } else {
                CLIUtils::error("Unknown option found", StringUtils::fromChar(optopt));
            }
        }
        default: /* shouldn't happen... */
            CLIUtils::error("An unknown error occurred while parsing command line arguments!");
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

    // finally, set default values and check numerical argument validity and return arguments if all goes well
    setDefaults(args);
    checkValidity(args);

    SPDLOG_DEBUG("Finished parsing command line arguments.");
}