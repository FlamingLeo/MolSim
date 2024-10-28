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

/// @brief Particle class modeling a particle's position, velocity, force, mass and type.
class Particle {
  private:
    /// @brief Position \f$ x \f$ of the particle.
    std::array<double, 3> x;

    /// @brief Velocity \f$ v \f$ of the particle.
    std::array<double, 3> v;

    /// @brief Force \f$ F \f$ effective on this particle.
    std::array<double, 3> f;

    /// @brief Force \f$ F_\text{old} \f$ which was effective on this particle.
    std::array<double, 3> old_f;

    /// @brief Mass \f$ m \f$ of this particle.
    double m;

    /// @brief Type of the particle. Use it for whatever you want (e.g. to separate molecules belonging to different
    /// bodies, matters, and so on).
    int type;

  public:
    /// @brief Construct a new Particle object by passing its type. Prevents implicit conversions.
    /// @param type The type of the Particle object to be constructed.
    explicit Particle(int type = 0);

    /**
     * @brief Construct a new Particle object using another Particle's data.
     *
     * @param other A reference to the Particle object to copy the data from.
     */
    Particle(const Particle &other);

    /**
     * @brief Construct a new Particle object using explicit values for each data field.
     *
     * @param x_arg A reference to the array containing data for the position \f$ x \f$.
     * @param v_arg A reference to the array containing data for the velocity \f$ v \f$.
     * @param m_arg The mass \f$ m \f$ of the particle.
     * @param type The type of the particle.
     */
    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        const std::array<double, 3> &x_arg, const std::array<double, 3> &v_arg, double m_arg, int type = 0);

    /// @brief Destroys the Particle object.
    virtual ~Particle();

    /**
     * @brief Gets the position \f$ x \f$ of this particle.
     *
     * @return A reference to the position array of this particle.
     */
    const std::array<double, 3> &getX() const;

    /**
     * @brief Gets the velocity \f$ v \f$ of this particle.
     *
     * @return A reference to the velocity array of this particle.
     */
    const std::array<double, 3> &getV() const;

    /**
     * @brief Gets the force \f$ F \f$ effective on this particle.
     *
     * @return A reference to the force array of this particle.
     */
    const std::array<double, 3> &getF() const;

    /**
     * @brief Gets the force \f$ F_\text{old} \f$ previously effective on this particle.
     *
     * @return A reference to the old force array of this particle.
     */
    const std::array<double, 3> &getOldF() const;

    /**
     * @brief Gets the mass \f$ m \f$ of the particle.
     *
     * @return The mass of the particle.
     */
    const double getM() const;

    /**
     * @brief Gets the type of the particle.
     *
     * @return The type of the particle.
     */
    const int getType() const;

    /**
     * @brief Sets the new position \f$ x \f$ of the particle to a given value.
     *
     * @param new_x A reference to the array containing the new position of this particle.
     */
    void setX(const std::array<double, 3> &new_x);

    /**
     * @brief Sets the new velocity \f$ v \f$ of the particle to a given value.
     *
     * @param new_v A reference to the array containing the new velocity of this particle.
     */
    void setV(const std::array<double, 3> &new_v);

    /**
     * @brief Sets the new force effective on the particle \f$ F \f$ to a given value.
     *
     * @param g A reference to the array containing the new force effective on this particle.
     */
    void setF(const std::array<double, 3> &g);

    /**
     * @brief Sets the new previously effective force on the particle \f$ F_\text{old} \f$ to a given value.
     *
     * @param g A reference to the array containing the new previously effective force on this particle.
     */
    void setOldF(const std::array<double, 3> &g);

    /// @brief Resets the force of the particle \f$ F \f$ to 0,0,0.
    void setFToZero();

    /**
     * @brief Returns a string representation of this particle.
     *
     * @return The formatted particle data as a std::string.
     */
    std::string toString() const;

    /**
     * @brief Overload of the equality operator for Particle objects.
     *
     * @param other A reference to the Particle object to compare the current object with.
     * @return true if both particles' data (attributes) contain the same values.
     * @return false if both particles have at least one differing attribute.
     */
    bool operator==(Particle &other);
};

/**
 * @brief Overload of the << operator for output streams, used primarily with string streams or console output.
 *
 * @param stream The output stream to pipe the Particle data into.
 * @param p A reference to the piped Particle.
 * @return The modified output stream, to allow chaining << operations.
 */
std::ostream &operator<<(std::ostream &stream, Particle &p);
