#include "Verlet.h"

#include "io/output/WriterFactory.h"
#include "utils/Arguments.h"
#include "utils/ArrayUtils.h"

Verlet::Verlet(const Arguments &args)
    : m_start_time{args.start_time}, m_end_time{args.end_time}, m_delta_t{args.delta_t}, m_it_freq{args.it_freq},
      m_type{args.type} {};
Verlet::~Verlet() = default;

void Verlet::runSimulation() {
    auto writer = outputWriter::createWriter(m_type);
    double current_time = m_start_time;
    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < m_end_time) {
        calculateX();
        calculateF();
        calculateV();

        iteration++;
        if (iteration % m_it_freq == 0)
            writer->writeParticles(m_particles, iteration);

        current_time += m_delta_t;
    }
}

void Verlet::calculateF() {
    // auto iterator = m_particles.begin();

    for (auto &p1 : m_particles) {
        p1.setOldF(p1.getF());
        p1.setFToZero();
        for (auto &p2 : m_particles) {
            // where i index is p1 and j index is p2
            if (not(p1 == p2)) {
                p1.setF(p1.getF() + ArrayUtils::elementWiseScalarOp(
                                        p1.getM() * p2.getM() / std::pow(ArrayUtils::L2Norm(p1.getX() - p2.getX()), 3),
                                        p2.getX() - p1.getX(), std::multiplies<>()));
            }
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