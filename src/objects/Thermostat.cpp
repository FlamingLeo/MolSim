#include "Thermostat.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <cmath>
#include <functional>
#include <spdlog/spdlog.h>

// helper methods for double inequality
static bool areNotEqual(double a, double b, double epsilon = 1e-9) { return std::fabs(a - b) > epsilon; }
static bool areEqual(double a, double b, double epsilon = 1e-9) { return std::fabs(a - b) <= epsilon; }

/* constructors and destructors */
Thermostat::Thermostat(ParticleContainer &particles) : particles{particles} {
    SPDLOG_TRACE("Created new Thermostat, arguments uninitialized!");
}
Thermostat::Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat, double T_target,
                       double delta_T, bool initBrownianMotion)
    : particles{particles}, dimension{dimension}, T_init{T_init}, T_target{std::isfinite(T_target) ? T_target : T_init},
      n_thermostat{n_thermostat}, delta_T{delta_T}, limitScaling{std::isfinite(delta_T)},
      initBrownianMotion{initBrownianMotion} {
    SPDLOG_TRACE("Created new Thermostat - dimensions: {}, T_init: {}, n_thermostat: {}, T_target: {}, delta_T: {}, "
                 "limit: {}, bm: {}",
                 this->dimension, this->T_init, this->n_thermostat, this->T_target, this->delta_T, limitScaling,
                 this->initBrownianMotion);
}
Thermostat::~Thermostat() = default;

/* functionality */
void Thermostat::initialize(int dimension, double T_init, int n_thermostat, double T_target, double delta_T,
                            bool initBrownianMotion) {
    this->dimension = dimension;
    this->T_init = T_init;
    this->n_thermostat = n_thermostat;
    this->T_target = std::isfinite(T_target) ? T_target : T_init;
    this->delta_T = delta_T;
    this->initBrownianMotion = initBrownianMotion;
    this->limitScaling = std::isfinite(delta_T);

    SPDLOG_TRACE("Initialized Thermostat - dimensions: {}, T_init: {}, n_thermostat: {}, T_target: {}, delta_T: {}, "
                 "limit: {}, bm: {}",
                 this->dimension, this->T_init, this->n_thermostat, this->T_target, this->delta_T, limitScaling,
                 this->initBrownianMotion);
}
void Thermostat::initializeBrownianMotion() {
    SPDLOG_TRACE("0-th iteration, initializing velocities with Brownian motion...");
    for (auto &p : particles) {
        // should probably check for active particles here, but we can assume that all of them are active at the
        // beginning of a simulation
        p.setV(maxwellBoltzmannDistributedVelocity(std::sqrt(T_init / p.getM()), dimension));
    }
}
void Thermostat::calculateKineticEnergy() {
    double sum = 0;
    for (auto &p : particles) {
        if (p.isActive()) {
            sum += p.getM() * ArrayUtils::L2NormSquared(p.getV());
        }
    }
    kineticEnergy = sum / 2;
    SPDLOG_TRACE("New kinetic energy: {}", kineticEnergy);
}
void Thermostat::calculateTemp() {
    temperature = 2 * kineticEnergy / (dimension * particles.activeSize());
    SPDLOG_TRACE("New temperature: {}", temperature);
}
void Thermostat::calculateScalingFactor() {
    // avoid division by 0
    if (areEqual(temperature, 0)) {
        scalingFactor = 1;
        SPDLOG_TRACE("Temperature was 0, setting scaling factor to 1.");
        return;
    }

    // if dTemp is specified, only increment by (at most) dTemp
    // otherwise, directly set temperature
    double diff = T_target - temperature;
    if (limitScaling && std::abs(diff) > delta_T)
        diff = (diff > 0) ? delta_T : -delta_T;
    double T_new = temperature + diff;

    scalingFactor = std::sqrt(T_new / temperature);
    SPDLOG_TRACE("New scaling factor: {}", scalingFactor);
}
void Thermostat::updateSystemTemp(int currentStep) {
    if (currentStep % n_thermostat != 0)
        return;

    // perform temperature update if the current iteration number is a multiple of n_thermostat
    SPDLOG_TRACE("Updating system temperature for iteration {} (n_thermostat: {})", currentStep, n_thermostat);

    // for the first iteration, initialize velocities with brownian motion (if set), then quit
    if (currentStep == 0) {
        if (initBrownianMotion) {
            initializeBrownianMotion();
        }
        return;
    }

    // calculate temperature based on kinetic energy and scale temperature
    calculateKineticEnergy();
    calculateTemp();

    // if the current temp. is the same as the target temp., skip updating velocities
    if (areEqual(temperature, T_target)) {
        SPDLOG_TRACE("Target temperature and current temperature are equal, skipping update.");
        return;
    }

    // calculate beta
    calculateScalingFactor();

    // update particle velocities to set new temperature
    for (auto &p : particles) {
        if (p.isActive()) {
            std::array<double, 3> newV = ArrayUtils::elementWiseScalarOp(scalingFactor, p.getV(), std::multiplies<>());
            p.setV(newV);
        }
    }
    SPDLOG_TRACE("Finished temperature update for iteration {}", currentStep);
}
double Thermostat::getKineticEnergy() const { return kineticEnergy; }
double Thermostat::getTemp() const { return temperature; }
double Thermostat::getInitTemp() const { return T_init; }
double Thermostat::getTargetTemp() const { return T_target; }
double Thermostat::getDeltaT() const { return delta_T; }
double Thermostat::getScalingFactor() const { return scalingFactor; }
int Thermostat::getTimestep() const { return n_thermostat; }
ParticleContainer &Thermostat::getParticles() const { return particles; }

bool Thermostat::operator==(const Thermostat &other) const {
    return dimension == other.dimension && kineticEnergy == other.kineticEnergy && temperature == other.temperature &&
           scalingFactor == other.scalingFactor && T_init == other.T_init && T_target == other.T_target &&
           n_thermostat == other.n_thermostat && delta_T == other.delta_T && limitScaling == other.limitScaling &&
           initBrownianMotion == other.initBrownianMotion;
}