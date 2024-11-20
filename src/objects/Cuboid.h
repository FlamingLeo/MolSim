/**
 * @file Cuboid.h
 * @brief Class storing metadata for a 2D / 3D Particle cuboid.
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "ParticleContainer.h"
#include <array>
#include <string>

/// @brief Cuboid class storing meta-data about the given cuboids.
class Cuboid {
  private:
    /// @brief Lower-left corner \f$ x \f$ of the cuboid.
    std::array<double, 3> x;

    /// @brief 3D Dimensions \f$ N \f$ of the cuboid.
    std::array<size_t, 3> N;

    /// @brief Distance \f$ h \f$ between the particles in the cuboid.
    double h;

    /// @brief Mass \f$ m \f$ of the particles in the cuboid.
    double m;

    /// @brief Starting velocity \f$ v \f$ of the particles in the cuboid (before added Maxwell-Boltzmann
    /// perturbations).
    std::array<double, 3> v;

    /// @brief Mean velocity for the Maxwell-Boltzmann distribution.
    double mean_velocity;

    /// @brief Reference to the ParticleContainer object from Lennard-Jones Simulation
    /// (passed by ParticleGenerator).
    ParticleContainer &particles;

  public:
    /**
     * @brief Constructs a new Cuboid object with all the needed meta-data.
     *
     * @param particles A reference to the central ParticleContainer from Lennard-Jones
     * @param x A reference to the array containing data for the cuboid position \f$ x \f$
     * @param N A reference to the array containing data for the dimensions \f$ N \f$ of the cuboid
     * @param v A reference to the array containing data for the starting speed \f$ v \f$ of the particles in the cuboid
     * @param h The distance \f$ h \f$ between the particles in the cuboid
     * @param m The mass \f$ m \f$ of the particles in the cuboid
     */
    Cuboid(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<size_t, 3> &N,
           const std::array<double, 3> &v, double h, double m);

    /* getters */
    /**
     * @brief Gets the position \f$ x \f$ of this cuboid.
     *
     * @return A reference to the position array of this cuboid.
     */
    const std::array<double, 3> &getX() const;

    /**
     * @brief Gets the dimensions \f$ N \f$ of this cuboid.
     *
     * @return A reference to the dimensions array of this cuboid.
     */
    const std::array<size_t, 3> &getN() const;

    /**
     * @brief Gets the distance \f$ h \f$ between the particles of this cuboid.
     *
     * @return The distance between the particles of the cuboid.
     */
    const double getH() const;

    /**
     * @brief Gets the mass \f$ m \f$ of the particles of this cuboid.
     *
     * @return The mass of the particles of the cuboid.
     */
    const double getM() const;

    /**
     * @brief Gets the starting velocity \f$ v \f$ of the particles of this cuboid.
     *
     * @return A reference of the velocity array of the cuboid.
     */
    const std::array<double, 3> &getV() const;

    /**
     * @brief Gets the mean velocity for the Maxwell-Boltzmann distribution.
     *
     * @return The mean velocity field of the cuboid.
     */
    const double getMeanVelocity() const;

    /**
     * @brief Gets the reference to the central ParticleContainer.
     *
     * @return The reference for the ParticleContainer stored in this container.
     */
    const ParticleContainer &getParticles() const;

    /* utility */
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
    void initializeParticles(size_t dimensions = 2);

    /**
     * @brief Returns a string representation of this Cuboid.
     *
     * @return The formatted Cuboid data as a std::string.
     */
    std::string toString() const;
};