#include "XMLReader.h"
#include "io/xsd/SimulationXSD.h"
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
#define LOAD_ARGSS(_x, _y)                                                                                             \
    if (xmlArgs._x().present()) {                                                                                      \
        args._x = xmlArgs._x().get();                                                                                  \
        args.argsSet.set(_y);                                                                                          \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, args._x);                                                                   \
    }
#define LOAD_ARGS2(_x, _y)                                                                                             \
    if (xmlArgs._x().present()) {                                                                                      \
        args._y = xmlArgs._x().get();                                                                                  \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, args._y);                                                                   \
    }
#define LOAD_ARGST(_x)                                                                                                 \
    if (xmlArgs._x().present()) {                                                                                      \
        args.type = StringUtils::toWriterType(xmlArgs._x().get());                                                     \
        SPDLOG_DEBUG("Loaded {}: {}", #_x, StringUtils::fromWriterType(args.type));                                    \
    }

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

        // read simulation parameters into arguments
        if (xmlInput->args().present()) {
            SPDLOG_TRACE("Found Arguments, loading...");
            const auto &xmlArgs = xmlInput->args().get();
            LOAD_ARGSS(startTime, 0);
            LOAD_ARGSS(endTime, 1);
            LOAD_ARGSS(delta_t, 2);
            LOAD_ARGS(epsilon);
            LOAD_ARGS(sigma);
            LOAD_ARGS2(frequency, itFreq);
            LOAD_ARGSS(basename, 3);
            LOAD_ARGST(output);
        }

        // get mandatory simulation type
        args.sim = StringUtils::toSimulationType(xmlInput->type());
        SPDLOG_DEBUG("Loaded simulation: {}", xmlInput->type());

        // load objects into particle container (wip)
        const auto &xmlObjects = xmlInput->objects();
        const auto &xmlCuboids = xmlObjects.cuboid();

        struct CuboidData {
            std::array<double, 3> x;
            std::array<double, 3> v;
            std::array<size_t, 3> N;
            double h;
            double m;
        } cuboidData;
        size_t particlesSize = pc.size();
        std::vector<CuboidData>
            cuboidDataVec; // TODO(scfl): add object / cuboid count to XML to reserve space beforehand

        for (auto &c : xmlCuboids) {
            cuboidData.x = {c.position().x(), c.position().y(), c.position().z()};
            cuboidData.v = {c.velocity().x(), c.velocity().y(), c.velocity().z()};
            cuboidData.N = {static_cast<size_t>(c.size().x()), static_cast<size_t>(c.size().y()),
                            static_cast<size_t>(c.size().z())}; // TODO(scfl): clean this up
            cuboidData.h = c.distance();
            cuboidData.m = c.mass();

            SPDLOG_TRACE("Read object data - x : {}, v : {}, N : {}, h : {}, m : {}.",
                         ArrayUtils::to_string(cuboidData.x), ArrayUtils::to_string(cuboidData.v),
                         ArrayUtils::to_string(cuboidData.N), cuboidData.h, cuboidData.m);

            cuboidDataVec.push_back(cuboidData);
            particlesSize += cuboidData.N[0] * cuboidData.N[1] * cuboidData.N[2];
        }

        // reserve enough space for _all_ particles
        pc.reserve(particlesSize);

        // initialize cuboid particles
        for (size_t i = 0; i < cuboidDataVec.size(); ++i) {
            Cuboid C{
                pc, cuboidDataVec[i].x, cuboidDataVec[i].N, cuboidDataVec[i].v, cuboidDataVec[i].h, cuboidDataVec[i].m};
            C.initializeParticles();
        }

        SPDLOG_DEBUG("Finished parsing XML file.");
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