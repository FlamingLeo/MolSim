#include "CLIParser.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <iostream>
#include <cstdlib>
#include <getopt.h>

/* could also do "using namespace CLIUtils" here... */
CLIParser::CLIParser(const char *progname)
{
    CLIUtils::filename = progname;
}

void CLIParser::checkValidity()
{
    // maybe disallow start and end being the same? or print out some warning?
    if (args.start_time > args.end_time)
        CLIUtils::error("Start time must be before end time!");
    if (args.delta_t <= 0)
        CLIUtils::error("Timestep must be positive!");
}

Arguments CLIParser::parseArguments(int argc, char **argv)
{
    static struct option long_options[]{
        {"start_time", required_argument, 0, 's'},
        {"end_time", required_argument, 0, 'e'},
        {"delta_t", required_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int ch;
    opterr = 0; // silence default getopt error messages

    // check for invalid syntax (not enough args)
    if (argc < 2)
        CLIUtils::error("Not enough arguments!");

    // loop over all of the options
    // TODO: fix partial options being recognized (e.g. "--he" for "--help")
    while ((ch = getopt_long(argc, argv, OPTSTRING, long_options, 0)) != -1)
    {
        switch (ch)
        {
        case 's':
            args.start_time = StringUtils::convertStringToDouble(optarg);
            break;
        case 'e':
            args.end_time = StringUtils::convertStringToDouble(optarg);
            break;
        case 'd':
            args.delta_t = StringUtils::convertStringToDouble(optarg);
            break;
        case 'h':
            CLIUtils::printHelp();
            std::exit(EXIT_SUCCESS);
        case '?':
            if (optopt == 's')
                CLIUtils::error("Start time not specified!");
            else if (optopt == 'e')
                CLIUtils::error("End time not specified!");
            else if (optopt == 'd')
                CLIUtils::error("Timestep not specified!");
            else
                CLIUtils::error("Unknown option found!");
        default:
            CLIUtils::error("An unknown error occurred while parsing command line arguments!");
        }
    }

    // by this point, following correct syntax, optind should be exactly one less than the argument counter
    if (optind != (argc - 1))
        CLIUtils::error("Invalid syntax!");

    // finally, check numerical argument validity and return arguments if all goes well
    checkValidity();
    return args;
}

Arguments CLIParser::getArgs() {
    return args;
};