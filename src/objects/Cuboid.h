/**
 * @file Cuboid.h
 * @brief Class storing metadata for a 2D / 3D Particle cuboid.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Cluster.h"
#include "ParticleContainer.h"
#include <array>
#include <string>

/// @brief Cuboid class storing meta-data about the given cuboids.
class Cuboid : public Cluster {
  private:
    /// @brief 3D Dimensions \f$ N \f$ of the cuboid.
    std::array<size_t, 3> N;

    /// @brief The stiffness constant \f$ k \f$ for membrane simulations.
    double k;

    /// @brief The average bond length \f$ r_0 \f$ for membrane simulations.
    double r_0;

    /// @brief The constant upward force \f$ F_{Z-UP} \f$ for membrane simulations.
    double fzup;

  public:
    /**
     * @brief Constructs a new Cuboid object with all the needed meta-data.
     *
     * @param particles A reference to the central ParticleContainer.
     * @param x A reference to the array containing data for the cuboid position \f$ x \f$.
     * @param N A reference to the array containing data for the dimensions \f$ N \f$ of the cuboid.
     * @param v A reference to the array containing data for the starting speed \f$ v \f$ of the particles in the
     * cuboid.
     * @param h The distance \f$ h \f$ between the particles in the cuboid.
     * @param m The mass \f$ m \f$ of the particles in the cuboid.
     * @param type The type of the particle.
     * @param epsilon The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     * @param k The stiffness constant \f$ k \f$, used for membrane simulations.
     * @param r_0 The average bond length \f$ r_0 \f$, used for membrane simulations.
     * @param fzup The constant upward force \f$ F_{Z-UP} \f$, used for membrane simulations.
     */
    Cuboid(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<size_t, 3> &N,
           const std::array<double, 3> &v, double h, double m, int type = TYPE_DEFAULT,
           double epsilon = EPSILON_DEFAULT, double sigma = SIGMA_DEFAULT, double k = K_DEFAULT,
           double r_0 = R0_DEFAULT, double fzup = FZUP_DEFAULT);

    /**
     * @brief Gets the dimensions \f$ N \f$ of this cuboid.
     *
     * @return A reference to the dimensions array of this cuboid.
     */
    std::array<size_t, 3> &getN();

    /**
     * @brief Gets the dimensions \f$ N \f$ of this cuboid (const).
     *
     * @return A const reference to the dimensions array of this cuboid.
     */
    const std::array<size_t, 3> &getN() const;

    /**
     * @brief Overload of the equality operator for Cuboid objects.
     *
     * @param other A reference to the Cuboid object to compare the current object with.
     * @return true if both cuboids' data (attributes) contain the same values.
     * @return false if both cuboids have at least one differing attribute.
     */
    bool operator==(const Cuboid &other) const;

    /**
     * @brief Overload of the inequality operator for Cuboid objects.
     *
     * @param other A reference to the Cuboid object to compare the current object with.
     * @return true if both particles have at least one differing attribute.
     * @return false if both particles' data (attributes) contain the same values.
     */
    bool operator!=(const Cuboid &other) const;

    /**
     * @brief Main function of Cuboid class, initializes the particles based on the cuboid meta-data and adds them to
     * the given ParticleContainer.
     *
     * @param dimensions The number of dimensions for which to perform the simulation. Must be either 1 or 2.
     */
    void initialize(size_t dimensions = 2) override;

    /**
     * @brief Returns a string representation of this Cuboid.
     *
     * @return The formatted Cuboid data as a std::string.
     */
    std::string toString() const;
};