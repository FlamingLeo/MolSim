#include "Verlet.h"
#include "io/output/WriterFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <string>

Verlet::Verlet(const ParticleContainer &pc, const Arguments &args)
    : m_particles{pc}, m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t},
      m_itFreq{args.itFreq}, m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with ParticleContainer {} and Arguments {}", pc.toString(),
                 args.toString());
}
Verlet::Verlet(const std::string &filename, const Arguments &args)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    m_particles.fromFile(filename);
    SPDLOG_TRACE("Created Verlet simulation from file {} with Arguments {}", filename, args.toString());
}
Verlet::Verlet(const Arguments &args)
    : m_startTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created Verlet simulation with Arguments {}", args.toString());
}
Verlet::~Verlet() { SPDLOG_TRACE("Destroyed Verlet object."); }

void Verlet::runSimulation() {
    SPDLOG_DEBUG("Running Verlet simulation (entered function)...");

    if (m_particles.isEmpty())
        CLIUtils::error("Cannot run simulation without particles!", "", false);

    auto writer = createWriter(m_type);
    double current_time = m_startTime;
    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running Verlet simulation with the following arguments:");
    SPDLOG_INFO("t_0         : {}", m_startTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < m_endTime) {
        calculateX();
        calculateF_thirdLaw();
        calculateV();

        iteration++;
        if (iteration % m_itFreq == 0) {
            // logging done here because otherwise the console output would legitimately become unreadable
            // this also makes it somewhat more configurable at runtime
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, current_time);
            writer->writeParticles(m_particles, iteration);
        }

        current_time += m_delta_t;
    }

    SPDLOG_INFO("Completed Verlet simulation.");
}

void Verlet::calculateF() {
    for (auto &p1 : m_particles) {
        p1.setOldF(p1.getF());
        p1.setFToZero();
        for (auto &p2 : m_particles) {
            // where i index is p1 and j index is p2
            if (!(p1 == p2)) {
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                        p1.getM() * p2.getM() / std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3),
                                        p2.getX() - p1.getX(), std::multiplies<>()));
            }
        }
    }
}

void Verlet::calculateF_thirdLaw() {
    // set previous f for each particle and reinitialize to zero
    for (auto &p1 : m_particles) {
        p1.setOldF(p1.getF());
        p1.setFToZero();
    }

    // loop over unique pairs
    for (size_t i = 0; i < m_particles.size(); ++i) {
        auto &p1 = m_particles[i];

        // avoid recalculating force for pairs which have already been computed
        for (size_t j = i + 1; j < m_particles.size(); ++j) {
            auto &p2 = m_particles[j];

            auto distVec = p2.getX() - p1.getX();
            double distNorm = ArrayUtils::L2Norm(distVec);

            // prevent division by 0
            if (distNorm == 0)
                continue;

            double forceMag = p1.getM() * p2.getM() / std::pow(distNorm, 3);
            auto forceVec = ArrayUtils::elementWiseScalarOp(forceMag, distVec, std::multiplies<>());

            // use newton's third law to apply force on p1 and opposite force on p2
            p1.setF(p1.getF() + forceVec);
            p2.setF(p2.getF() - forceVec);
        }
    }
}

void Verlet::calculateX() {
    for (auto &p : m_particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(m_delta_t, p.getV(), std::multiplies<>()) +
               m_delta_t * m_delta_t *
                   ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
    }
}

void Verlet::calculateV() {
    for (auto &p : m_particles) {
        p.setV(p.getV() + ArrayUtils::elementWiseScalarOp(m_delta_t / (2 * p.getM()), p.getOldF() + p.getF(),
                                                          std::multiplies<>()));
    }
}

ParticleContainer &Verlet::getParticles() { return m_particles; }