#include "Cluster.h"
#include <spdlog/spdlog.h>

Cluster::Cluster(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<double, 3> &v, double h,
                 double m, int type, double epsilon, double sigma)
    : particles{particles}, x{x}, v{v}, h{h}, m{m}, type{type}, epsilon{epsilon}, sigma{sigma}, mean_velocity{0.1} {
    SPDLOG_TRACE("Generated Cluster.");
}
Cluster::~Cluster() = default;

std::array<double, 3> &Cluster::getX() { return x; }
const std::array<double, 3> &Cluster::getX() const { return x; }
double Cluster::getH() const { return h; }
double Cluster::getM() const { return m; }
std::array<double, 3> &Cluster::getV() { return v; }
const std::array<double, 3> &Cluster::getV() const { return v; }
double Cluster::getMeanVelocity() const { return mean_velocity; }
ParticleContainer &Cluster::getParticles() { return particles; }
const ParticleContainer &Cluster::getParticles() const { return particles; }