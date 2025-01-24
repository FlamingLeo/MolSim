#include "Cuboid.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Cuboid::Cuboid(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<size_t, 3> &N,
               const std::array<double, 3> &v, double h, double m, int type, double epsilon, double sigma, double k,
               double r_0, double fzup)
    : Cluster(particles, x, v, h, m, type, epsilon, sigma), N{N}, k{k}, r_0{r_0}, fzup{fzup} {
    SPDLOG_TRACE("Generated Cuboid (simple constructor) - x: {}, N: {}, h: {}, m: {}, v: {}, mean_v: {}",
                 ArrayUtils::to_string(x), ArrayUtils::to_string(N), h, m, ArrayUtils::to_string(v), mean_velocity);
}


void Cuboid::initialize(size_t dimensions) {
    SPDLOG_TRACE("Initializing Particles for Cuboid {}...", this->toString());
    //we don't initialize with special forces
    if (fzup == 0) {
        std::array<double, 3> xyz;
        for (size_t i = 0; i < N[2]; i++) {
            for (size_t j = 0; j < N[1]; j++) {
                for (size_t k = 0; k < N[0]; k++) {
                    xyz = {x[0] + k * h, x[1] + j * h, x[2] + i * h};
                    v = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, dimensions),
                                                      std::plus<>());
                    particles.addParticle(xyz, v, m, type, epsilon, sigma, this->k, r_0, fzup);
                }
            }
        }

    } else{
        std::array<double, 3> xyz;
        for (size_t i = 0; i < N[2]; i++) {
            for (size_t j = 0; j < N[1]; j++) {
                for (size_t k = 0; k < N[0]; k++) {
                    xyz = {x[0] + k * h, x[1] + j * h, x[2] + i * h};
                    v = ArrayUtils::elementWisePairOp(v, maxwellBoltzmannDistributedVelocity(mean_velocity, dimensions),
                                                      std::plus<>());
                    if(specialCase(k, j, i)) {
                        particles.addParticle(xyz, v, m, type, epsilon, sigma, this->k, r_0, fzup);
                    } else{
                        particles.addParticle(xyz, v, m, type, epsilon, sigma, this->k, r_0, 0);
                    }
                }
            }
        }
    }
}


void Cuboid::initializeNeighbours() {
    //currently only implemented for 2d membranes
    SPDLOG_TRACE("Initializing Particle Neighbours for Cuboid {}...", this->toString());
    //we can use this because initializeNeighbours is called right after initialize
    int startIndex = particles.size() - N[0] * N[1] * N[2];

    for (size_t i = 0; i < N[2]; i++) {
        for (size_t j = 0; j < N[1]; j++) {
            for (size_t k = 0; k < N[0]; k++) {

                std::vector<std::reference_wrapper<Particle>> direct;
                std::vector<std::reference_wrapper<Particle>> diagonal;
                int ownIndex = startIndex + i * (N[0] * N[1]) + j * N[0] + k;

                //add direct neighbours, minding edge particles
                //left
                if(k != 0)
                    direct.emplace_back(particles.get(ownIndex - 1));
                //right
                if(k != N[0] - 1)
                    direct.emplace_back(particles.get(ownIndex + 1));
                //down
                if(j != 0)
                    direct.emplace_back(particles.get(ownIndex - N[0]));
                //up
                if(j != N[1] - 1)
                    direct.emplace_back(particles.get(ownIndex + N[0]));

                //add diagonal neighbours, minding edge particles
                //upper left
                if(k != 0 && j != N[1] - 1)
                    diagonal.emplace_back(particles.get(ownIndex + N[0] - 1));
                //lower left
                if(k != 0 && j != 0)
                    diagonal.emplace_back(particles.get(ownIndex - N[0] - 1));
                //upper right
                if(k != N[0] - 1 && j != N[1] - 1)
                    diagonal.emplace_back(particles.get(ownIndex + N[0] + 1));
                //lower right
                if(k != N[0] - 1 && j != 0)
                    diagonal.emplace_back(particles.get(ownIndex - N[0] + 1));

                particles.get(ownIndex).setDirectNeighbours(direct);
                particles.get(ownIndex).setDiagonalNeighbours(diagonal);
                SPDLOG_DEBUG("Particle {} has {} direct neighbours and {} diagonal neighbours",
                             particles.get(ownIndex).toString(), particles.get(ownIndex).getDirectNeighbours().size(),
                             particles.get(ownIndex).getDiagonalNeighbours().size());
            }
        }
    }
}

bool Cuboid::specialCase(int x, int y, int z){
    //this function tells whether the particle should have a special force fz applied to it
    //currently hard-coded
    if(x == 17 && y == 24)
        return true;
    if(x == 17 && y == 25)
        return true;
    if(x == 18 && y == 24)
        return true;
    if(x == 18 && y == 25)
        return true;
    return false;
}

std::array<size_t, 3> &Cuboid::getN() { return N; }
const std::array<size_t, 3> &Cuboid::getN() const { return N; }
bool Cuboid::operator==(const Cuboid &other) const {
    return (x == other.x) && (N == other.N) && (h == other.h) && (m == other.m) && (v == other.v) &&
           (mean_velocity == other.mean_velocity) && (particles == other.particles) && (sigma == other.sigma) &&
           (epsilon == other.epsilon);
}
bool Cuboid::operator!=(const Cuboid &other) const { return !(*this == other); }
std::string Cuboid::toString() const {
    std::stringstream stream;
    stream << "{ x: " << x << ", N: " << N << ", v: " << v << ", h: " << h << ", m: " << m << ", sigma: " << sigma
           << ", epsilon: " << epsilon << ", particles: " << particles.toString() << " }";
    return stream.str();
}
