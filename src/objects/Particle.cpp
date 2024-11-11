#include "Particle.h"
#include "utils/ArrayUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

Particle::Particle(int type_arg) : type{type_arg} {
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    SPDLOG_TRACE("Generated Particle (simple constructor) - x: {}, v: {}, f: {}, m: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m);
}

Particle::Particle(const Particle &other)
    : x{other.x}, v{other.v}, f{other.f}, old_f{other.old_f}, m{other.m}, type{other.type} {
    SPDLOG_TRACE("Generated Particle (copy) - x: {}, v: {}, f: {}, m: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m);
}

Particle::Particle(const std::array<double, 3> &x_arg, const std::array<double, 3> &v_arg, double m_arg, int type_arg)
    : x{x_arg}, v{v_arg}, f{0., 0., 0.}, old_f{0., 0., 0.}, m{m_arg}, type{type_arg} {
    SPDLOG_TRACE("Generated Particle (arguments) - x: {}, v: {}, f: {}, m: {}", ArrayUtils::to_string(x),
                 ArrayUtils::to_string(v), ArrayUtils::to_string(f), m);
}

Particle::~Particle() {
    SPDLOG_TRACE("Destroyed Particle - x: {}, v: {}, f: {}, m: {}", ArrayUtils::to_string(x), ArrayUtils::to_string(v),
                 ArrayUtils::to_string(f), m);
}

const std::array<double, 3> &Particle::getX() const { return x; }
const std::array<double, 3> &Particle::getV() const { return v; }
const std::array<double, 3> &Particle::getF() const { return f; }
const std::array<double, 3> &Particle::getOldF() const { return old_f; }
double Particle::getM() const { return m; }
int Particle::getType() const { return type; }

void Particle::setX(const std::array<double, 3> &new_x) { x = new_x; }
void Particle::setV(const std::array<double, 3> &new_v) { v = new_v; }
void Particle::setF(const std::array<double, 3> &g) { f = g; }
void Particle::setOldF(const std::array<double, 3> &g) { old_f = g; }
void Particle::setFToZero() { std::fill(std::begin(f), std::end(f), 0); }

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "{ x: " << x << ", v: " << v << ", f: " << f << ", old_f: " << old_f << ", m: " << m << ", type: " << type
           << " }";
    return stream.str();
}

bool Particle::operator==(const Particle &other) const {
    return (x == other.x) && (v == other.v) && (f == other.f) && (type == other.type) && (m == other.m) &&
           (old_f == other.old_f);
}

bool Particle::operator!=(const Particle &other) const {
    return (x != other.x) || (v != other.v) || (f != other.f) || (type != other.type) || (m != other.m) ||
           (old_f != other.old_f);
}

std::ostream &operator<<(std::ostream &stream, const Particle &p) {
    stream << p.toString();
    return stream;
}
