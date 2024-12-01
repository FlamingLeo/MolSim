#include "Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/CLIUtils.h"
#include <cmath>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#define MASS_ERROR "The mass of a particle must be positive for the currently available simulations!"

Particle::Particle(int type_arg)
    : x{0., 0., 0.}, v{0., 0., 0.}, f{0., 0., 0.}, old_f{0., 0., 0.}, m{1.}, type{type_arg}, cellIndex{-1} {
    SPDLOG_TRACE("Generated Particle (simple constructor) - x: {}, v: {}, f: {}, m: {}, cellIndex: {}",
                 ArrayUtils::to_string(x), ArrayUtils::to_string(v), ArrayUtils::to_string(f), m, cellIndex);
    if (m <= 0)
        CLIUtils::error(MASS_ERROR);
}

Particle::Particle(const Particle &other)
    : x{other.x}, v{other.v}, f{other.f}, old_f{other.old_f}, m{other.m}, type{other.type}, cellIndex{other.cellIndex} {
    SPDLOG_TRACE("Generated Particle (copy) - x: {}, v: {}, f: {}, m: {}, cellIndex: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m, cellIndex);
}

Particle::Particle(const std::array<double, 3> &x_arg, const std::array<double, 3> &v_arg, double m_arg, int type_arg)
    : x{x_arg}, v{v_arg}, f{0., 0., 0.}, old_f{0., 0., 0.}, m{m_arg}, type{type_arg}, cellIndex{-1} {
    SPDLOG_TRACE("Generated Particle (arguments) - x: {}, v: {}, f: {}, m: {}, cellIndex: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m, cellIndex);
    if (m <= 0)
        CLIUtils::error(MASS_ERROR);
}

Particle::~Particle() {
    SPDLOG_TRACE("Destroyed Particle - x: {}, v: {}, f: {}, m: {}, cellIndex: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m, cellIndex);
}

const std::array<double, 3> &Particle::getX() const { return x; }
const std::array<double, 3> &Particle::getV() const { return v; }
const std::array<double, 3> &Particle::getF() const { return f; }
const std::array<double, 3> &Particle::getOldF() const { return old_f; }
const double Particle::getM() const { return m; }
const int Particle::getType() const { return type; }
const int Particle::getCellIndex() const { return cellIndex; }
const bool Particle::isActive() const { return active; }

void Particle::setX(const std::array<double, 3> &new_x) { x = new_x; }
void Particle::setV(const std::array<double, 3> &new_v) { v = new_v; }
void Particle::setF(const std::array<double, 3> &g) { f = g; }
void Particle::setOldF(const std::array<double, 3> &g) { old_f = g; }
void Particle::setFToZero() { std::fill(std::begin(f), std::end(f), 0); }
void Particle::setM(double new_m) {
    if (new_m <= 0)
        CLIUtils::error(MASS_ERROR, "", false);
    m = new_m;
}
void Particle::setType(double new_type) { type = new_type; }
void Particle::setCellIndex(int new_index) { cellIndex = new_index; }
void Particle::markInactive() {
    SPDLOG_TRACE("Marking particle {} as inactive...", this->toString());
    v = {0, 0, 0};
    f = {0, 0, 0};
    old_f = {0, 0, 0};
    active = false;
}

std::string Particle::toString() const {
    std::stringstream ss;
    ss << "{ x: " << x << ", v: " << v << ", f: " << f << ", old_f: " << old_f << ", m: " << m << ", type: " << type
       << ", cellIndex:" << cellIndex << ", active: " << active << " }";
    return ss.str();
}

bool Particle::operator==(const Particle &other) const {
    return (x == other.x) && (v == other.v) && (f == other.f) && (type == other.type) && (m == other.m) &&
           (old_f == other.old_f) && (active == other.active);
}

bool Particle::operator!=(const Particle &other) const {
    return (x != other.x) || (v != other.v) || (f != other.f) || (type != other.type) || (m != other.m) ||
           (old_f != other.old_f) && (active == other.active);
}

std::ostream &operator<<(std::ostream &stream, const Particle &p) {
    stream << p.toString();
    return stream;
}
