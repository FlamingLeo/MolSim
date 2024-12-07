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

#define TYPE_DEFAULT 0
#define SIGMA_DEFAULT 1
#define EPSILON_DEFAULT 5

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

    /// @brief Type of the particle.
    int type;

    /// @brief Depth \f$ \epsilon \f$ of the potential well. Lennard-Jones parameter.
    double epsilon;

    /// @brief Distance \f$ \sigma \f$ where the Lennard-Jones potential reaches zero. Lennard-Jones parameter.
    double sigma;

    /// @brief The cell index of the particle, to be used with the linked cell method.
    int cellIndex;

    /**
     * @brief The status of the particle.
     *
     * @details If the particle is marked "active", it will be considered in the time integration calculation.
     * Otherwise, it is ignored.
     *
     */
    bool active{true};

  public:
    /**
     * @brief Construct a new Particle object by optionally passing its type. Prevents implicit conversions.
     *
     * Given that the current simulations make use of the particle mass as the divisor in some formulas, the mass may
     * not be negative or 0.
     *
     * @param type The type of the Particle object to be constructed.
     */
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
     * Given that the current simulations make use of the particle mass as the divisor in some formulas, the mass may
     * not be negative or 0.
     *
     * @param x A reference to the array containing data for the position \f$ x \f$.
     * @param v A reference to the array containing data for the velocity \f$ v \f$.
     * @param m The mass \f$ m \f$ of the particle.
     * @param type The type of the particle.
     * @param eps The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     */
    Particle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m, int type = TYPE_DEFAULT,
             double eps = EPSILON_DEFAULT, double sigma = SIGMA_DEFAULT);

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
     * @brief Gets the Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     *
     * @return The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     */
    const double getEpsilon() const;

    /**
     * @brief Gets the Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     *
     * @return The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     */
    const double getSigma() const;

    /**
     * @brief Get the index of the particle in a CellContainer.
     *
     * For use with the linked cell method.
     *
     * @return The index of the particle in a grid of cells.
     */
    const int getCellIndex() const;

    /**
     * @brief Checks if the particle is currently active.
     *
     * @return true if the particle is currently active.
     * @return false if the particle is currently inactive.
     */
    const bool isActive() const;

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
     * @param new_f A reference to the array containing the new force effective on this particle.
     */
    void setF(const std::array<double, 3> &new_f);

    /**
     * @brief Sets the new previously effective force on the particle \f$ F_\text{old} \f$ to a given value.
     *
     * @param new_old_f A reference to the array containing the new previously effective force on this particle.
     */
    void setOldF(const std::array<double, 3> &new_old_f);

    /// @brief Resets the force of the particle \f$ F \f$ to 0,0,0.
    void setFToZero();

    /**
     * @brief Sets the new mass \f$ m \f$ of the particle to a given value.
     *
     * Given that the current simulations make use of the particle mass as the divisor in some formulas, the mass may
     * not be negative or 0.
     *
     * @param new_m The new mass of this particle.
     */
    void setM(double new_m);

    /**
     * @brief Sets the new type of the particle to a given value.
     *
     * @param new_type The new type of this particle.
     */
    void setType(double new_type);

    /**
     * @brief Sets the new Lennard-Jones parameter \f$ \epsilon \f$ of the particle to a given value.
     *
     * @param new_eps The new Lennard-Jones parameter \f$ \epsilon \f$ of this particle.
     */
    void setEpsilon(double new_eps);

    /**
     * @brief Sets the new Lennard-Jones parameter \f$ \sigma \f$ of the particle to a given value.
     *
     * @param new_sigma The new Lennard-Jones parameter \f$ \sigma \f$ of this particle.
     */
    void setSigma(double new_sigma);

    /**
     * @brief Sets the new index in a CellContainer.
     *
     * For use with the linked cell method.
     *
     * @param new_index The new index in a grid of cells.
     */
    void setCellIndex(int new_index);

    /// @brief Sets the Particle's active status to "inactive".
    void markInactive();

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
    bool operator==(const Particle &other) const;

    /**
     * @brief Overload of the inequality operator for Particle objects.
     *
     * @param other A reference to the Particle object to compare the current object with.
     * @return true if both particles have at least one differing attribute.
     * @return false if both particles' data (attributes) contain the same values.
     */
    bool operator!=(const Particle &other) const;
};

/**
 * @brief Overload of the << operator for output streams, used primarily with string streams or console output.
 *
 * @param stream The output stream to pipe the Particle data into.
 * @param p A reference to the piped Particle.
 * @return The modified output stream, to allow chaining << operations.
 */
std::ostream &operator<<(std::ostream &stream, const Particle &p);
