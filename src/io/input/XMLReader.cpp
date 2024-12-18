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
    (void)0;
}

void XMLReader::readXML(Arguments &args, ParticleContainer &pc, Thermostat &t) {
    args.startTime = 0;
    args.endTime = 25;
    args.delta_t = 0.0005;
    args.itFreq = 10;
    args.basename = "MD_vtk_LJ_W4T2_L";
    args.type = WriterType::VTK;
    args.domainSize = {300., 54., 1};
    args.cutoffRadius = 3;
    args.conditions = {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::PERIODIC,
                       BoundaryCondition::PERIODIC,   BoundaryCondition::OUTFLOW,    BoundaryCondition::OUTFLOW};
    args.gravity = -12.44;
    args.sim = SimulationType::LJ;
    args.linkedCells = true;

    t.initialize(2, 40, 1000, 40, INFINITY, true);

    pc.reserve(10000);
    Cuboid c1{pc, {3.6, 5., 0.}, {250, 20, 1}, {0, 0, 0}, 1.2, 1.0, 1, 1.0, 1.2};
    Cuboid c2{pc, {3.6, 30., 0.}, {250, 20, 1}, {0, 0, 0}, 1.2, 2.0, 1, 1.0, 1.1};
    c1.initialize();
    c2.initialize();
}