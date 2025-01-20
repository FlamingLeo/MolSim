#include "XMLWriter.h"
#include "io/xsd/SimulationXSD.h"
#include "utils/CLIUtils.h"
#include "utils/CellUtils.h"
#include "utils/StringUtils.h"
#include <spdlog/spdlog.h>
#include <string>

XMLWriter::XMLWriter(const std::string &filename) : m_filename{filename} {
    openFile(filename);
    SPDLOG_TRACE("Created new XMLWriter with file {}.", filename);
}
XMLWriter::~XMLWriter() {
    closeFile();
    SPDLOG_TRACE("Destroyed XMLWriter.");
}
void XMLWriter::closeFile() {
    if (m_file.is_open()) {
        m_file.close();
        SPDLOG_DEBUG("Closed file.");
    } else {
        SPDLOG_TRACE("No file to close.");
    }
}
void XMLWriter::openFile(const std::string &filename) {
    if (m_file.is_open())
        m_file.close();

    m_file.open(filename);

    if (!m_file.is_open())
        CLIUtils::error("Failed to open file", filename, "", false);

    SPDLOG_DEBUG("Opened file {} for writing.", filename);
}
void XMLWriter::serialize(const ParticleContainer &pc, const Arguments &args, const Thermostat &t) {
    if (!(m_file.is_open()))
        CLIUtils::error("No output file opened!", "", false);

    // serialize complete simulation arguments
    ArgsType a{};
    a.startTime() = args.startTime;
    a.endTime() = args.endTime;
    a.delta_t() = args.delta_t;
    a.frequency() = args.itFreq;
    a.basename() = args.basename;
    a.output() = StringUtils::fromWriterType(args.type);
    a.cutoffRadius() = args.cutoffRadius;
    a.domainSize() = {args.domainSize[0], args.domainSize[1], args.domainSize[2]};
    a.bdConditions() = {
        CellUtils::fromBoundaryCondition(args.conditions[0]), CellUtils::fromBoundaryCondition(args.conditions[1]),
        CellUtils::fromBoundaryCondition(args.conditions[2]), CellUtils::fromBoundaryCondition(args.conditions[3]),
        CellUtils::fromBoundaryCondition(args.conditions[4]), CellUtils::fromBoundaryCondition(args.conditions[5])};
    a.gravity() = args.gravity;
    a.parallelization() = StringUtils::fromParallelizationType(args.parallelization);

    // serialize thermostat
    ThermostatType tt{t.getTemp(), t.getTimestep()};
    tt.brownianMotion() = false; // since this is a continuation, we don't reinitialize velocities
    tt.nanoFlow() = t.getNanoflow();
    tt.target() = t.getTargetTemp(); // no need to check if t_target is finite, because it always is...
    if (std::isfinite(t.getDeltaT()))
        tt.deltaT() = t.getDeltaT();

    // serialize each molecule inside the particle container
    ObjectsType o{};
    for (auto &p : pc) {
        // skip inactive particles
        if (!p.isActive())
            continue;

        // serialize particle data
        ParticleType pt{PositionType{p.getX()[0], p.getX()[1], p.getX()[2]},
                        VelocityType{p.getV()[0], p.getV()[1], p.getV()[2]}, p.getM()};
        pt.force() = {p.getF()[0], p.getF()[1], p.getF()[2]};
        pt.oldForce() = {p.getOldF()[0], p.getOldF()[1], p.getOldF()[2]};
        pt.type() = p.getType();
        pt.epsilon() = p.getEpsilon();
        pt.sigma() = p.getSigma();
        pt.cellIndex() = p.getCellIndex();

        // add particle to object sequence
        o.particle().push_back(pt);
    }

    // finalize output
    SimType s{tt, StringUtils::fromSimulationType(args.sim), o};
    s.args(a);
    s.linkedCells() = args.linkedCells;
    s.totalParticles() = pc.activeSize();
    s.dimensions() = args.dimensions;

    // potentially serialize membrane data
    if (args.membrane && !pc.isEmpty()) {
        // this can be expanded if the values differ for each particle
        MembraneType m{pc[0].getK(), pc[0].getR0(), pc[0].getFZUP()};
        s.membrane() = m;
    }

    // write output to file
    sim(m_file, s);
    SPDLOG_INFO("Saved final simulation state to output file {}.", m_filename);
}
