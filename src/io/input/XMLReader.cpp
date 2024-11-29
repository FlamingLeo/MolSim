#include "XMLReader.h"
#include "io/xsd/SimulationXSD.h"
#include "objects/Cuboid.h"
#include "objects/Disc.h"
#include "objects/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <array>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#define LOAD_ARGS(_x)                                                                                                  \
    if (xmlArgs._x().present()) {                                                                                      \
        args._x = xmlArgs._x().get();                                                                                  \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, args._x);                                                                   \
    }
#define LOAD_ARGS_SET(_x, _y)                                                                                          \
    if (xmlArgs._x().present()) {                                                                                      \
        args._x = xmlArgs._x().get();                                                                                  \
        args.argsSet.set(_y);                                                                                          \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, args._x);                                                                   \
    }
#define LOAD_ARGS_2(_x, _y)                                                                                            \
    if (xmlArgs._x().present()) {                                                                                      \
        args._y = xmlArgs._x().get();                                                                                  \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, args._y);                                                                   \
    }
#define LOAD_ARGS_TYPE(_x)                                                                                             \
    if (xmlArgs._x().present()) {                                                                                      \
        args.type = StringUtils::toWriterType(xmlArgs._x().get());                                                     \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, StringUtils::fromWriterType(args.type));                                    \
    }
#define LOAD_ARGS_3DARR(_x)                                                                                            \
    if (xmlArgs._x().present()) {                                                                                      \
        const auto &arr = xmlArgs._x().get();                                                                          \
        args._x = {arr.x(), arr.y(), arr.z()};                                                                         \
        SPDLOG_TRACE("Loaded {}: {}", ArrayUtils::to_string(args._x));                                                 \
    }

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
        LOAD_ARGS_SET(startTime, 0);
        LOAD_ARGS_SET(endTime, 1);
        LOAD_ARGS_SET(delta_t, 2);
        LOAD_ARGS(epsilon);
        LOAD_ARGS(sigma);
        LOAD_ARGS_2(frequency, itFreq);
        LOAD_ARGS_SET(basename, 3);
        LOAD_ARGS_TYPE(output);
        // TODO: incorporate these once linked cells are fixed
        LOAD_ARGS_3DARR(domainSize);
        LOAD_ARGS(cutoffRadius);
    }
}

// helper (wrapper) function to parse cuboids into a particle container
void parseCuboids(const SimType::ObjectsType &xmlObjects, ParticleContainer &pc) {
    for (const auto &cuboid : xmlObjects.cuboid()) {
        std::array<double, 3> position{cuboid.position().x(), cuboid.position().y(), cuboid.position().z()};
        std::array<double, 3> velocity{cuboid.velocity().x(), cuboid.velocity().y(), cuboid.velocity().z()};
        std::array<size_t, 3> size{static_cast<size_t>(cuboid.size().x()), static_cast<size_t>(cuboid.size().y()),
                                   static_cast<size_t>(cuboid.size().z())};
        double distance = cuboid.distance();
        double mass = cuboid.mass();

        SPDLOG_TRACE("Initializing cuboid with x: {}, v: {}, N: {}, h: {}, m: {}", ArrayUtils::to_string(position),
                     ArrayUtils::to_string(velocity), ArrayUtils::to_string(size), distance, mass);

        Cuboid cuboidObj{pc, position, size, velocity, distance, mass};
        cuboidObj.initializeParticles();
    }
}

// helper (wrapper) function to parse single particles into a particle container
void parseParticles(const SimType::ObjectsType &xmlObjects, ParticleContainer &pc) {
    for (const auto &particle : xmlObjects.particle()) {
        std::array<double, 3> position{particle.position().x(), particle.position().y(), particle.position().z()};
        std::array<double, 3> velocity{particle.velocity().x(), particle.velocity().y(), particle.velocity().z()};
        double mass = particle.mass();
        int type = particle.type();

        SPDLOG_TRACE("Adding particle with x: {}, v: {}, m: {}, type: {}", ArrayUtils::to_string(position),
                     ArrayUtils::to_string(velocity), mass, type);

        pc.addParticle(Particle(position, velocity, mass, type));
    }
}

// helper (wrapper) function to parse discs into a particle container
void parseDiscs(const SimType::ObjectsType &xmlObjects, ParticleContainer &pc) {
    for (const auto &disc : xmlObjects.disc()) {
        std::array<double, 3> position{disc.position().x(), disc.position().y(), disc.position().z()};
        std::array<double, 3> velocity{disc.velocity().x(), disc.velocity().y(), disc.velocity().z()};
        int radius = disc.radius();
        double distance = disc.distance();
        double mass = disc.mass();

        SPDLOG_TRACE("Initializing disc with x: {}, v: {}, r: {}, h: {}, m: {}", ArrayUtils::to_string(position),
                     ArrayUtils::to_string(velocity), radius, distance, mass);

        Disc discObj{pc, position, radius, velocity, distance, mass};
        discObj.initializeDisc();
    }
}

/* main xml functionality starts here... */
XMLReader::XMLReader(const std::string &filename) {
    SPDLOG_TRACE("Generated XMLReader from file {}", filename);
    openFile(filename);
}

void XMLReader::readXML(Arguments &args, ParticleContainer &pc) {
    if (!m_infile.is_open())
        CLIUtils::error("No file opened for reading!", "", false);

    try {
        SPDLOG_DEBUG("Reading from XML file...");
        std::unique_ptr<SimType> xmlInput{sim(m_infile, xml_schema::Flags::dont_validate)};

        reserveParticleContainer(pc, xmlInput);
        readXMLArgs(args, xmlInput);

        args.sim = StringUtils::toSimulationType(xmlInput->type());
        SPDLOG_DEBUG("Loaded simulation: {}", xmlInput->type());

        const auto &xmlObjects = xmlInput->objects();
        size_t initialParticles = pc.size(); // might come in handy?

        parseCuboids(xmlObjects, pc);
        parseParticles(xmlObjects, pc);
        parseDiscs(xmlObjects, pc);

        if (pc.size() == 0)
            CLIUtils::error("No particles added!", "", false);

        SPDLOG_DEBUG("Finished parsing XML file. Total particles: {}", pc.size() - initialParticles);
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