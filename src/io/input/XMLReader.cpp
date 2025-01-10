#include "XMLReader.h"
#include "objects/Cuboid.h"
#include "objects/Disc.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/CellUtils.h"
#include "utils/StringUtils.h"
#include <array>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

/* main xml functionality starts here... */
XMLReader::XMLReader(const std::string &filename) {
    SPDLOG_TRACE("Generated XMLReader from file {}", filename);
    openFile(filename);
}

void XMLReader::readXML(Arguments &args, ParticleContainer &pc, Thermostat &t) {
    /* ADJUSTED FOR ICPX COMPATIBILITY */
    args.startTime = 0;
    args.endTime = 0.5;
    args.gravity = -12.44;
    args.delta_t = 0.0005;
    args.itFreq = 0x7FFF;
    args.domainSize = {300, 54, 1};
    args.cutoffRadius = 3;
    args.conditions = {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::PERIODIC,
                       BoundaryCondition::PERIODIC,   BoundaryCondition::PERIODIC,   BoundaryCondition::PERIODIC};
    args.basename = "MD_contest";
    args.sim = SimulationType::LJ;
    args.linkedCells = true;

    t.initialize(2, 40.0, 1000, INFINITY, INFINITY, true);

    Cuboid c1{pc, {3.6, 5.0, 0.0}, {250, 20, 1}, {0., 0., 0.}, 1.2, 1.0, 1, 1.0, 1.2};
    Cuboid c2{pc, {3.6, 30.0, 0.0}, {250, 20, 1}, {0., 0., 0.}, 1.2, 2.0, 1, 1.0, 1.1};
    c1.initialize();
    c2.initialize();
}