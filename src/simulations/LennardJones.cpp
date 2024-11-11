//
// Created by marag on 11/5/2024.
//
#include "LennardJones.h"
#include "io/output/WriterFactory.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/StringUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

LennardJones::LennardJones(const std::string &filename, const Arguments &args)
    : m_particles{ParticleContainer()}, m_generator{CuboidGenerator(filename, m_particles)}, m_epsilon{5}, m_sigma{1},
      m_currentTime{args.startTime}, m_endTime{args.endTime}, m_delta_t{args.delta_t}, m_itFreq{args.itFreq},
      m_type{args.type} {
    SPDLOG_TRACE("Created LJ Simulation from file {} with Arguments {}", filename, args.toString());
}
LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::runSimulation() {
    SPDLOG_TRACE("Running LennardJones simulation (entered function)...");

    auto writer = createWriter(m_type);
    int iteration = 0;

    // log user choices
    SPDLOG_INFO("Running LJ simulation with the following arguments:");
    SPDLOG_INFO("epsilon     : {}", m_epsilon);
    SPDLOG_INFO("sigma       : {}", m_sigma);
    SPDLOG_INFO("t_0         : {}", m_currentTime);
    SPDLOG_INFO("t_end       : {}", m_endTime);
    SPDLOG_INFO("dt          : {}", m_delta_t);
    SPDLOG_INFO("Output Freq.: every {} iterations", m_itFreq);
    SPDLOG_INFO("Output Type : {}", StringUtils::fromWriterType(m_type));

    // initialize particles from cuboid data
    m_generator.generateCuboids();

    while (m_currentTime < m_endTime) {
        calculateX();
        LJForce();
        calculateV();

        iteration++;
        if (iteration % m_itFreq == 0) {
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, m_currentTime);
            writer->writeParticles(m_particles, iteration);
        }

        m_currentTime += m_delta_t;
    }

    SPDLOG_INFO("Completed LJ simulation.");
}

void LennardJones::LJForce() {
    for (auto &p1 : m_particles) {
        p1.setOldF(p1.getF());
        p1.setFToZero();
        for (auto &p2 : m_particles) {
            if (!(p1 == p2)) {
                double L2_Norm = ArrayUtils::L2Norm(p1.getX() - p2.getX());
                p1.setF(p1.getF() +
                        ArrayUtils::elementWiseScalarOp(((-24 * m_epsilon) / std::pow(L2_Norm, 2)) *
                                                            (std::pow((m_sigma / std::pow(L2_Norm, 2)), 6) -
                                                             2 * std::pow((m_sigma / std::pow(L2_Norm, 2)), 12)),
                                                        p1.getX() - p2.getX(), std::multiplies<>()));
            }
        }
    }
}

void LennardJones::calculateX() {
    for (auto &p : m_particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(m_delta_t, p.getV(), std::multiplies<>()) +
               m_delta_t * m_delta_t *
                   ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
    }
}

void LennardJones::calculateV() {
    for (auto &p : m_particles) {
        p.setV(p.getV() + ArrayUtils::elementWiseScalarOp(m_delta_t / (2 * p.getM()), p.getOldF() + p.getF(),
                                                          std::multiplies<>()));
    }
}