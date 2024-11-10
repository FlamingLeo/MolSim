//
// Created by marag on 11/5/2024.
//
#include "LennardJones.h"
#include "io/output/WriterFactory.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <functional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

LennardJones::LennardJones(const std::string &filename, const Arguments &args)
    : particles{ParticleContainer()}, generator{CuboidGenerator(filename, particles)} {
    SPDLOG_TRACE("Created LJ Simulation");
};

int epsilon = 5;
int sigma = 1;
double current_time = 0;
double end_time = 5;
double delta_t = 0.0002;

LennardJones::~LennardJones() { SPDLOG_TRACE("Destroyed LJ object."); }

void LennardJones::runSimulation() {
    SPDLOG_DEBUG("Running LennardJones simulation (entered function)...");

    auto writer = createWriter(WriterType::VTK);
    int iteration = 0;

    std::vector<Cuboid> cuboids = generator.generateCuboids();


    while (current_time < end_time) {
        calculateX();
        LJ_Force();
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            // logging done here because otherwise the console output would legitimately become unreadable
            // this also makes it somewhat more configurable at runtime
            SPDLOG_TRACE("Iteration: {}, t_i: {}", iteration, current_time);
            writer->writeParticles(particles, iteration);
        }

        current_time += delta_t;
    }

    SPDLOG_INFO("Completed LJ simulation.");
}

void LennardJones::LJ_Force() {
    for (auto &p1 : particles) {
        p1.setOldF(p1.getF());
        p1.setFToZero();
        for (auto &p2 : particles) {
            if (!(p1 == p2)) {
                double L2_Norm = ArrayUtils::L2Norm(p1.getX() - p2.getX());
                if(std::isnan(L2_Norm)){
                    //SPDLOG_INFO("NaN");
                    //SPDLOG_INFO(p1.toString());
                    //SPDLOG_INFO(p2.toString());
                }
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                ((-24 * epsilon)/std::pow(L2_Norm, 2)) * 
                (std::pow((sigma/std::pow(L2_Norm, 2)), 6) - 2 * std::pow((sigma/std::pow(L2_Norm, 2)), 12)),
                p1.getX() - p2.getX(), std::multiplies<>()));
            }
        }
        //SPDLOG_INFO(p1.toString());
    }
}

void LennardJones::calculateX() {
    for (auto &p : particles) {
        p.setX(p.getX() + ArrayUtils::elementWiseScalarOp(delta_t, p.getV(), std::multiplies<>()) +
               delta_t * delta_t * ArrayUtils::elementWiseScalarOp(1 / (2 * p.getM()), p.getF(), std::multiplies<>()));
    }
}

void LennardJones::calculateV() {
    for (auto &p : particles) {
        p.setV(p.getV() +
               ArrayUtils::elementWiseScalarOp(delta_t / (2 * p.getM()), p.getOldF() + p.getF(), std::multiplies<>()));
        std::array<double, 3> new_v = {p.getV()[0], p.getV()[1], 0};
        p.setV(new_v);
    }
}