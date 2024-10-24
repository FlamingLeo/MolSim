/**
 * @file Particle.h
 * @author eckhardw
 * @brief Model of a single particle.
 * @date 2010-02-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <array>
#include <string>

/// @brief Particle class modeling a particle's position, velocity, force, mass
/// and type.
class Particle {

  private:
    /**
     * @brief Position of the particle.
     */
    std::array<double, 3> x;

    /**
     * @brief Velocity of the particle.
     */
    std::array<double, 3> v;

    /**
     * @brief Force effective on this particle.
     */
    std::array<double, 3> f;

    /**
     * @brief Force which was effective on this particle.
     */
    std::array<double, 3> old_f;

    /**
     * @brief Mass of this particle.
     */
    double m;

    /**
     * @brief Type of the particle. Use it for whatever you want (e.g. to
     * separate molecules belonging to different bodies, matters, and so on).
     *
     */
    int type;

  public:
    explicit Particle(int type = 0);

    Particle(const Particle &other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
        int type = 0);

    virtual ~Particle();

    const std::array<double, 3> &getX() const;
    const std::array<double, 3> &getV() const;
    const std::array<double, 3> &getF() const;
    const std::array<double, 3> &getOldF() const;
    const double getM() const;
    const int getType() const;

    void setX(const std::array<double, 3> &new_x);
    void setV(const std::array<double, 3> &new_v);
    void setF(const std::array<double, 3> &g);
    void setOldF(const std::array<double, 3> &g);

    /// @brief Resets the force of the particle to 0,0,0.
    void setFToZero();

    std::string toString() const;

    bool operator==(Particle &other);
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
