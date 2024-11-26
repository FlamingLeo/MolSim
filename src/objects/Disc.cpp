#include "Disc.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>



Disc::Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r,
               const std::array<double, 3> &v, double h, double m)
        : x{x}, r{r}, h{h}, m{m}, v{v}, mean_velocity{0.1}, particles{particles} {
    SPDLOG_TRACE("Generated Disc (simple constructor) - x: {}, r: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 ArrayUtils::to_string(x), r, h, m, ArrayUtils::to_string(v), mean_velocity);
}

void Disc::initializeDisc() {
    SPDLOG_TRACE("Initializing Particles for Disc {}...", this->toString());
    const double pi = 3.14159265358979323846;

    //we interpreted radius 1 as already a ring around the centre
    //radius 0 --> particle in the centre
    std::array<double, 3> pos = {0, 0, 0};
    std::array<double, 3> vel = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2),
                                      std::plus<>());
    particles.addParticle(pos, vel, m);

    if(r != 0) {

        //we generate particles in concentric circles
        for (int i = 0; i < r; i++) {

            //we take h as arc distance, not euclidean distance for convenience (arc > euclidean)
            double ringRadius = (i + 1) * h;
            double segments = 2 * pi * (i + 1);
            int points = std::floor(segments);

            //initialize the particles along the ring
            for (int p = 0; p < points; p++) {

                std::array<double, 3> xyz;
                double angle = (2 * pi * h / std::floor(segments)) * p;
                xyz[0] = x[0] + std::cos(angle) * ringRadius;
                xyz[1] = x[1] + std::sin(angle) * ringRadius;
                xyz[2] = 0;
                std::array<double, 3> velocity = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, 2),
                                                  std::plus<>());
                std::cout << xyz << "\n";
                particles.addParticle(xyz, velocity, m);

            }

        }
    }
}

std::array<double, 3> &Disc::getX() { return x; }
int Disc::getR() { return r; }
double Disc::getH() { return h; }
double Disc::getM() { return m; }
std::array<double, 3> &Disc::getV() { return v; }
double Disc::getMeanVelocity() { return mean_velocity; }
ParticleContainer &Disc::getParticles() { return particles; }

bool Disc::operator==(const Disc &other) const {
    return (x == other.x) && (r == other.r) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles);
}
bool Disc::operator!=(const Disc &other) const { return !(*this == other); }

std::string Disc::toString() {
    std::stringstream stream;
    stream << "{ x: " << x << ", r: " << r << ", v: " << v << ", h: " << h << ", m: " << m
           << ", particles: " << particles.toString() << " }";
    return stream.str();
}