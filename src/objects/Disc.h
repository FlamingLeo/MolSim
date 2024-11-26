/**
 * @file Disc.h
 * @brief Class storing metadata for a 2D Particle disc.
 * @date 2024-11-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "ParticleContainer.h"
#include <array>
#include <string>

/// @brief Cuboid class storing meta-data about the given cuboids.
class Disc {
private:
    /// @brief Centre \f$ x \f$ of the disc.
    std::array<double, 3> x;

    /// @brief Radius in \f$ r \f$ of the disc in particles
    int r;

    /// @brief Distance \f$ h \f$ between the particles in the disc.
    double h;

    /// @brief Mass \f$ m \f$ of the particles in the disc.
    double m;

    /// @brief Starting velocity \f$ v \f$ of the particles in the disc (before added Maxwell-Boltzmann
    /// perturbations).
    std::array<double, 3> v;

    /// @brief Mean velocity for the Maxwell-Boltzmann distribution.
    double mean_velocity;

    /// @brief Reference to the ParticleContainer object from Simulation
    /// (passed by DiscGenerator).
    ParticleContainer &particles;

public:
    /**
     * @brief Constructs a new Disc object with all the needed meta-data.
     *
     * @param particles A reference to the central ParticleContainer
     * @param x A reference to the array containing data for the disc position \f$ x \f$
     * @param r The radius in particles \f$ r \f$ of the disc
     * @param v A reference to the array containing data for the starting speed \f$ v \f$ of the particles in the disc
     * @param h The distance \f$ h \f$ between the particles in the disc
     * @param m The mass \f$ m \f$ of the particles in the disc
     */
    Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r,
           const std::array<double, 3> &v, double h, double m);

    /* getters */
    /**
     * @brief Gets the position \f$ x \f$ of this disc.
     *
     * @return A reference to the position array of this disc.
     */
    std::array<double, 3> &getX();

    /**
     * @brief Gets the radius in particles \f$ r \f$ of this disc.
     *
     * @return The radius of this disc.
     */
    int getR();

    /**
     * @brief Gets the distance \f$ h \f$ between the particles of this disc.
     *
     * @return The distance between the particles of the disc.
     */
    double getH();

    /**
     * @brief Gets the mass \f$ m \f$ of the particles of this disc.
     *
     * @return The mass of the particles of the disc.
     */
    double getM();

    /**
     * @brief Gets the starting velocity \f$ v \f$ of the particles of this disc.
     *
     * @return A reference of the velocity array of the disc.
     */
    std::array<double, 3> &getV();

    /**
     * @brief Gets the mean velocity for the Maxwell-Boltzmann distribution.
     *
     * @return The mean velocity field of the disc.
     */
    double getMeanVelocity();

    /**
     * @brief Gets the reference to the central ParticleContainer.
     *
     * @return The reference for the ParticleContainer stored in this container.
     */
    ParticleContainer &getParticles();

    /* utility */
    /**
     * @brief Overload of the equality operator for Cuboid objects.
     *
     * @param other A reference to the Disc object to compare the current object with.
     * @return true if both discs' data (attributes) contain the same values.
     * @return false if both discs have at least one differing attribute.
     */
    bool operator==(const Disc &other) const;

    /**
     * @brief Overload of the inequality operator for Disc objects.
     *
     * @param other A reference to the Disc object to compare the current object with.
     * @return true if both discs' have at least one differing attribute.
     * @return false if both discs' data (attributes) contain the same values.
     */
    bool operator!=(const Disc &other) const;

    /// @brief Main function of Disc class, initializes the particles based on the disc meta-data
    /// and adds them to the given ParticleContainer.
    void initializeDisc();

    /**
     * @brief Returns a string representation of this Disc.
     *
     * @return The formatted Disc data as a std::string.
     */
    std::string toString();
};
