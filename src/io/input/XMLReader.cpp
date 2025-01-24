#include "XMLReader.h"
#include "io/xsd/SimulationXSD.h"
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

#define LOAD_ARGS(_a, _b, _x)                                                                                          \
    if (_a._x().present()) {                                                                                           \
        _b._x = _a._x().get();                                                                                         \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, _b._x);                                                                     \
    }
#define LOAD_ARGS_SET(_a, _b, _c, _x, _y)                                                                              \
    if (_a._x().present()) {                                                                                           \
        _b._x = _a._x().get();                                                                                         \
        _b._c.set(_y);                                                                                                 \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, _b._x);                                                                     \
    }
#define LOAD_ARGS_2(_a, _b, _x, _y)                                                                                    \
    if (_a._x().present()) {                                                                                           \
        _b._y = _a._x().get();                                                                                         \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, _b._y);                                                                     \
    }
#define LOAD_ARGS_TYPE(_a, _b, _c, _x)                                                                                 \
    if (_a._x().present()) {                                                                                           \
        _b._c = StringUtils::toWriterType(_a._x().get());                                                              \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, StringUtils::fromWriterType(_b._c));                                        \
    }
#define LOAD_ARGS_3DARR(_a, _b, _x)                                                                                    \
    if (_a._x().present()) {                                                                                           \
        const auto &arr = _a._x().get();                                                                               \
        _b._x = {arr.x(), arr.y(), arr.z()};                                                                           \
    }
#define GET_IF_PRESENT(_a, _b, _c) (_a._b().present() ? _a._b().get() : _c)

// helper function for reserving a certain amount of spaces in a particle container beforehand, if present
static void reserveParticleContainer(ParticleContainer &pc, const std::unique_ptr<SimType> &xmlInput) {
    if (xmlInput->totalParticles().present()) {
        size_t totalParticles = static_cast<size_t>(xmlInput->totalParticles().get());
        pc.reserve(totalParticles);
        SPDLOG_DEBUG("Reserved space for {} particles...", totalParticles);
    }
}

// helper function to read xml args into arguments struct
static void readXMLArgs(Arguments &args, const std::unique_ptr<SimType> &xmlInput) {
    if (xmlInput->args().present()) {
        SPDLOG_TRACE("Found Arguments, loading...");
        const auto &xmlArgs = xmlInput->args().get();
        LOAD_ARGS_SET(xmlArgs, args, argsSet, startTime, 0);
        LOAD_ARGS_SET(xmlArgs, args, argsSet, endTime, 1);
        LOAD_ARGS_SET(xmlArgs, args, argsSet, delta_t, 2);
        LOAD_ARGS_2(xmlArgs, args, frequency, itFreq);
        LOAD_ARGS_SET(xmlArgs, args, argsSet, basename, 3);
        LOAD_ARGS_TYPE(xmlArgs, args, type, output);
        LOAD_ARGS_3DARR(xmlArgs, args, domainSize);
        LOAD_ARGS(xmlArgs, args, cutoffRadius);
        if (xmlArgs.bdConditions().present()) {
            auto bdConditions = xmlArgs.bdConditions().get();
            BoundaryCondition N = CellUtils::toBoundaryCondition(bdConditions.n());
            BoundaryCondition S = CellUtils::toBoundaryCondition(bdConditions.s());
            BoundaryCondition W = CellUtils::toBoundaryCondition(bdConditions.w());
            BoundaryCondition E = CellUtils::toBoundaryCondition(bdConditions.e());
            BoundaryCondition A = CellUtils::toBoundaryCondition(bdConditions.a());
            BoundaryCondition B = CellUtils::toBoundaryCondition(bdConditions.b());
            args.conditions = {N, S, W, E, A, B};
            SPDLOG_DEBUG("Loaded boundary conditions: n: {}, s: {}, w: {}, e: {}, a: {}, b: {}", bdConditions.n(),
                         bdConditions.s(), bdConditions.w(), bdConditions.e(), bdConditions.a(), bdConditions.b());
        }
        LOAD_ARGS(xmlArgs, args, gravity);
    }
}

// helper function to read and initialize thermostat
static void initThermostat(const SimType::ThermostatType &xmlThermostat, Thermostat &t, int dimensions) {
    t.initialize(dimensions, xmlThermostat.init(), xmlThermostat.timeStep(),
                 GET_IF_PRESENT(xmlThermostat, target, INFINITY), GET_IF_PRESENT(xmlThermostat, deltaT, INFINITY),
                 GET_IF_PRESENT(xmlThermostat, brownianMotion, true), GET_IF_PRESENT(xmlThermostat, nanoFlow, false));
}

// helper (wrapper) function to parse cuboids into a particle container
static void parseCuboids(const SimType::ObjectsType &xmlObjects, const SimType::MembraneOptional &membrane,
                         ParticleContainer &pc) {
    for (const auto &cuboid : xmlObjects.cuboid()) {
        std::array<double, 3> position{cuboid.position().x(), cuboid.position().y(), cuboid.position().z()};
        std::array<double, 3> velocity{cuboid.velocity().x(), cuboid.velocity().y(), cuboid.velocity().z()};
        std::array<size_t, 3> size{static_cast<size_t>(cuboid.size().x()), static_cast<size_t>(cuboid.size().y()),
                                   static_cast<size_t>(cuboid.size().z())};
        double distance = cuboid.distance();
        double mass = cuboid.mass();
        int type = cuboid.type().present() ? cuboid.type().get() : TYPE_DEFAULT;
        double epsilon = cuboid.epsilon().present() ? cuboid.epsilon().get() : EPSILON_DEFAULT;
        double sigma = cuboid.sigma().present() ? cuboid.sigma().get() : SIGMA_DEFAULT;
        double k = membrane.present() ? membrane.get().stiffness() : K_DEFAULT;
        double r_0 = membrane.present() ? membrane.get().avgBondLength() : R0_DEFAULT;
        double fzup = membrane.present() ? membrane.get().zForce() : FZUP_DEFAULT;

        SPDLOG_DEBUG("Initializing cuboid with x: {}, v: {}, N: {}, h: {}, m: {}, eps: {}, sigma: {}, k: {}, r_0: {}, "
                     "f_z-up: {}",
                     ArrayUtils::to_string(position), ArrayUtils::to_string(velocity), ArrayUtils::to_string(size),
                     distance, mass, epsilon, sigma, k, r_0, fzup);

        Cuboid cuboidObj{pc, position, size, velocity, distance, mass, type, epsilon, sigma, k, r_0, fzup};
        cuboidObj.initialize();
        if(membrane.present())
            cuboidObj.initializeNeighbours();
    }
}

// helper (wrapper) function to parse single particles into a particle container
static void parseParticles(const SimType::ObjectsType &xmlObjects, ParticleContainer &pc) {
    for (const auto &particle : xmlObjects.particle()) {
        std::array<double, 3> position{particle.position().x(), particle.position().y(), particle.position().z()};
        std::array<double, 3> velocity{particle.velocity().x(), particle.velocity().y(), particle.velocity().z()};
        std::array<double, 3> force, oldForce;

        // ternary operators don't work with arrays :/
        if (particle.force().present()) {
            force = {particle.force().get().x(), particle.force().get().y(), particle.force().get().z()};
        } else {
            force = {0., 0., 0.};
        }
        if (particle.oldForce().present()) {
            oldForce = {particle.oldForce().get().x(), particle.oldForce().get().y(), particle.oldForce().get().z()};
        } else {
            oldForce = {0., 0., 0.};
        }
        double mass = particle.mass();
        int type = particle.type().present() ? particle.type().get() : TYPE_DEFAULT;
        double epsilon = particle.epsilon().present() ? particle.epsilon().get() : EPSILON_DEFAULT;
        double sigma = particle.sigma().present() ? particle.sigma().get() : SIGMA_DEFAULT;
        int cellIndex = particle.cellIndex().present() ? particle.cellIndex().get() : -1;

        SPDLOG_TRACE(
            "Adding particle with x: {}, v: {}, f: {}, old_f: {}, m: {}, type: {}, eps: {}, sigma: {}, index: {}",
            ArrayUtils::to_string(position), ArrayUtils::to_string(velocity), ArrayUtils::to_string(force),
            ArrayUtils::to_string(oldForce), mass, type, epsilon, sigma, cellIndex);

        // membrane properties cannot be applied to singular particles
        pc.addParticle(position, velocity, force, oldForce, mass, type, epsilon, sigma, K_DEFAULT, R0_DEFAULT,
                       FZUP_DEFAULT, cellIndex);
    }
}

// helper (wrapper) function to parse discs into a particle container
static void parseDiscs(const SimType::ObjectsType &xmlObjects, ParticleContainer &pc) {
    for (const auto &disc : xmlObjects.disc()) {
        std::array<double, 3> position{disc.position().x(), disc.position().y(), disc.position().z()};
        std::array<double, 3> velocity{disc.velocity().x(), disc.velocity().y(), disc.velocity().z()};
        int radius = disc.radius();
        double distance = disc.distance();
        double mass = disc.mass();
        int type = disc.type().present() ? disc.type().get() : TYPE_DEFAULT;
        double epsilon = disc.epsilon().present() ? disc.epsilon().get() : EPSILON_DEFAULT;
        double sigma = disc.sigma().present() ? disc.sigma().get() : SIGMA_DEFAULT;

        SPDLOG_TRACE("Initializing disc with x: {}, v: {}, r: {}, h: {}, m: {}, eps: {}, sigma: {}",
                     ArrayUtils::to_string(position), ArrayUtils::to_string(velocity), radius, distance, mass, epsilon,
                     sigma);

        Disc discObj{pc, position, radius, velocity, distance, mass, type, epsilon, sigma};
        discObj.initialize();
    }
}

/* main xml functionality starts here... */
XMLReader::XMLReader(const std::string &filename) {
    SPDLOG_TRACE("Generated XMLReader from file {}", filename);
    openFile(filename);
}

void XMLReader::readXML(Arguments &args, ParticleContainer &pc, Thermostat &t) {
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!", "", false);

    try {
        SPDLOG_DEBUG("Reading from XML file...");
        std::unique_ptr<SimType> xmlInput{sim(m_infile, xml_schema::Flags::dont_validate)};

        reserveParticleContainer(pc, xmlInput);
        readXMLArgs(args, xmlInput);

        args.sim = StringUtils::toSimulationType(xmlInput->type());
        SPDLOG_DEBUG("Loaded simulation: {}", xmlInput->type());

        if (xmlInput->linkedCells().present()) {
            args.linkedCells = xmlInput->linkedCells().get();
            SPDLOG_DEBUG("Using linked cells?: {}", args.linkedCells);
        }

        if (xmlInput->dimensions().present()) {
            args.dimensions = xmlInput->dimensions().get();
        }
        SPDLOG_DEBUG("Number of dimensions: {}", args.dimensions);

        const auto &xmlThermostat = xmlInput->thermostat();
        initThermostat(xmlThermostat, t, args.dimensions);

        const auto &xmlObjects = xmlInput->objects();
        size_t initialParticles = pc.size(); // might come in handy?

        SimType::MembraneOptional &membrane = xmlInput->membrane();
        args.membrane = membrane.present();
        SPDLOG_DEBUG("Membrane simulation?: {}", args.membrane);

        parseCuboids(xmlObjects, membrane, pc);
        parseParticles(xmlObjects, pc);
        parseDiscs(xmlObjects, pc);

        if (pc.size() == 0)
            CLIUtils::error("No particles added!", "", false);

        SPDLOG_DEBUG("Finished parsing XML file. Total particles: {}.", pc.size() - initialParticles);
    } catch (const xml_schema::Exception &e) {
        std::stringstream ss;
        ss << e;
        std::string l;
        while (std::getline(ss, l, '\n')) {
            SPDLOG_ERROR("XMLReader: {}", l);
        }
        CLIUtils::error("Error parsing XML file!", "", false);
    }
}