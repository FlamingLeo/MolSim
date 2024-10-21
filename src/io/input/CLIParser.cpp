#include "CLIParser.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <iostream>
#include <cstdlib>
#include <getopt.h>

void CLIParser::checkValidity(const Arguments &args)
{
    // maybe disallow start and end being the same? or print out some warning?
    if (args.start_time > args.end_time)
        CLIUtils::error("Start time must be before end time!");
    if (args.delta_t <= 0)
        CLIUtils::error("Timestep must be positive!");
}

void CLIParser::parseArguments(int argc, char **argv, Arguments &args)
{
    int ch;                       // store option character
    CLIUtils::filename = argv[0]; // update filename for help and usage strings
    opterr = 0;                   // silence default getopt error messages

    // check for invalid syntax (not enough args)
    if (argc < 2)
        CLIUtils::error("Not enough arguments!");

    // loop over all of the options
    while ((ch = getopt(argc, argv, OPTSTRING)) != -1)
    {
        switch (ch)
        {
        case 's': /* start time */
            args.start_time = StringUtils::toDouble(optarg);
            break;
        case 'e': /* end time */
            args.end_time = StringUtils::toDouble(optarg);
            break;
        case 'd': /* timestep */
            args.delta_t = StringUtils::toDouble(optarg);
            break;
        case 'h': /* help */
            CLIUtils::printHelp();
            std::exit(EXIT_SUCCESS);
        case '?': /* invalid syntax */
            if (optopt == 's')
                CLIUtils::error("Start time not specified!");
            else if (optopt == 'e')
                CLIUtils::error("End time not specified!");
            else if (optopt == 'd')
                CLIUtils::error("Timestep not specified!");
            else
                CLIUtils::error("Unknown option found!");
        default: /* shouldn't happen... */
            CLIUtils::error("An unknown error occurred while parsing command line arguments!");
        }
    }

    // by this point, following correct syntax, optind should be exactly one less than the argument counter
    if (optind != (argc - 1))
        CLIUtils::error("Invalid syntax!");

    // finally, check numerical argument validity and return arguments if all goes well
    checkValidity(args);
}