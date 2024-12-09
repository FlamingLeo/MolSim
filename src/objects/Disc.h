/**
 * @file Disc.h
 * @brief Class storing metadata for a 2D Particle disc.
 * @date 2024-11-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Cluster.h"
#include "ParticleContainer.h"
#include <array>
#include <string>
#include <vector>

/**
 * @brief Class storing metadata about a Particle disc.
 *
 */
class Disc : public Cluster {
  private:
    /// @brief Radius \f$ r \f$ of the disc in terms of number of particles.
    int r;

    /**
     * @brief Helper function to get circle point (Particle) coordinates.
     *
     * @param centerX The central X coordinate.
     * @param centerY The central Y coordinate.
     * @param radius The radius of the circle.
     * @param distance The spacing between each point (Particle).
     * @return A vector containing the coordinate of each point (Particle).
     */
    std::vector<std::array<double, 3>> getCircleCoordinates(double centerX, double centerY, double radius,
                                                            double distance);

  public:
    /**
     * @brief Constructs a new Disc object with all the needed metadata.
     *
     * @param particles A reference to the central ParticleContainer.
     * @param x A reference to the array containing data for the disc position \f$ x \f$.
     * @param r The radius \f$ r \f$ of the disc.
     * @param v A reference to the array containing data for the starting speed \f$ v \f$ of the particles in the
     * disc.
     * @param h The distance \f$ h \f$ between the particles in the disc.
     * @param m The mass \f$ m \f$ of the particles in the disc.
     * @param type The type of the particle.
     * @param eps The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     */
    Disc(ParticleContainer &particles, const std::array<double, 3> &x, int r, const std::array<double, 3> &v, double h,
         double m, int type = TYPE_DEFAULT, double epsilon = EPSILON_DEFAULT, double sigma = SIGMA_DEFAULT);

    /**
     * @brief Gets the radius \f$ r \f$ of this disc.
     *
     * @return The radius of this disc.
     */
    int getR() const;

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

    /// @brief Main function of Disc class, initializes the particles based on the disc metadata
    /// in a grid and adds them to the given ParticleContainer.
    void initialize(size_t dimensions = 2) override;

    /**
     * @brief Returns a string representation of this Disc.
     *
     * @return The formatted Disc data as a std::string.
     */
    std::string toString();
};
