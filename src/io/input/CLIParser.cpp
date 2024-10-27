#include "CLIParser.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <unordered_map>

/// @brief Map containing conversion information for converting a string to a WriterType enum.
/// Static .cpp utility variable; undocumented in header file.
static std::unordered_map<std::string, WriterType> const writerTable = {{"vtk", WriterType::VTK},
                                                                        {"xyz", WriterType::XYZ}};

/// @brief Map containing conversion information for converting a string to a SimulationType enum.
/// Static .cpp utility variable; undocumented in header file.
static std::unordered_map<std::string, SimulationType> const SimulationTable = {{"verlet", SimulationType::VERLET}};

/// @brief Function to convert a string to a WriterType enum using the above map.
/// @param type The string containing the desired WriterType.
/// @return The desired WriterType enum if the type string is valid, otherwise terminate with error.
static WriterType stringToWriterType(const std::string &type) {
    auto it = writerTable.find(type);
    if (it != writerTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return WriterType::VTK; // shouldn't reach this; included to silence warning
}

/// @brief Function to convert a string to a SimulationType enum using the above map.
/// @param type The string containing the desired SimulationType.
/// @return The desired SimulationType enum if the type string is valid, otherwise terminate with error.
static SimulationType stringToSimulationType(const std::string &type) {
    auto it = SimulationTable.find(type);
    if (it != SimulationTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return SimulationType::VERLET; // shouldn't reach this; included to silence warning
}

/* documented functions start here */
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

    // check for invalid syntax (not enough args)
    if (argc < 2)
        CLIUtils::error("Not enough arguments! Use '-h' to display a help message.");

    // loop over all of the options
    while ((ch = getopt(argc, argv, OPTSTRING)) != -1) {
        switch (ch) {
        case 's': /* start time */
            args.startTime = StringUtils::toDouble(optarg);
            break;
        case 'e': /* end time */
            args.endTime = StringUtils::toDouble(optarg);
            break;
        case 'd': /* timestep */
            args.delta_t = StringUtils::toDouble(optarg);
            break;
        case 'f': /* output frequency */
            args.itFreq = StringUtils::toInt(optarg);
            break;
        case 'o': /* output type */
            args.type = stringToWriterType(optarg);
            break;
        case 't': /* simulation type */
            args.sim = stringToSimulationType(optarg);
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
            else if (optopt == 'f')
                CLIUtils::error("Output frequency not specified!");
            else if (optopt == 'o')
                CLIUtils::error("Output type not specified!");
            else if (optopt == 't')
                CLIUtils::error("Simulation type not specified!");
            else
                CLIUtils::error("Unknown option found", StringUtils::fromString(optopt));
        default: /* shouldn't happen... */
            CLIUtils::error("An unknown error occurred while parsing command "
                            "line arguments!");
        }
    }

    // handle getopt's special "--" argument
    if (std::strcmp(argv[optind - 1], "--") == 0)
        CLIUtils::error("Invalid option '--'!");

    // by this point, following correct syntax, optind should be exactly one less than the argument counter
    if (optind != (argc - 1))
        CLIUtils::error("Invalid syntax - no file input provided!");

    // finally, check numerical argument validity and return arguments if all goes well
    checkValidity(args);
}