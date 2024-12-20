/**
 * @file Cluster.h
 * @brief Abstract class for defining clusters of Particle objects.
 * @date 2024-12-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "ParticleContainer.h"
#include <array>

/// @brief Abstract class which models common information about Particle objects arranged in some cluster.
class Cluster {
  protected:
    /**
     * @brief Starting coordinate \f$ x \f$ of the Cluster.
     *
     * @details For Disc objects, it is the center of the disc. For Cuboid objects, it is the lower left corner of the
     * cuboid.
     *
     */
    std::array<double, 3> x;

    /// @brief Distance \f$ h \f$ between the particles in the Cluster.
    double h;

    /// @brief Mass \f$ m \f$ of the particles in the Cluster.
    double m;

    /// @brief Type of the particles in the Cluster.
    int type;

    /// @brief Depth \f$ \epsilon \f$ of the potential well of the particles in the Cluster.
    double epsilon;

    /// @brief Distance \f$ \sigma \f$ where the Lennard-Jones potential reaches zero of the particles in the Cluster.
    double sigma;

    /// @brief Starting velocity \f$ v \f$ of the particles in the Cluster (before adding Maxwell-Boltzmann
    /// perturbations).
    std::array<double, 3> v;

    /// @brief Mean velocity for the Maxwell-Boltzmann distribution.
    double mean_velocity;

    /// @brief Reference to the ParticleContainer object from the Simulation.
    ParticleContainer &particles;

  public:
    /**
     * @brief Constructs a new Cluster.
     *
     * @param particles A reference to the central ParticleContainer.
     * @param x A reference to the array containing data for the cluster position \f$ x \f$.
     * @param v A reference to the array containing data for the starting speed \f$ v \f$ of the particles in the
     * cluster.
     * @param h The distance \f$ h \f$ between the particles in the cluster.
     * @param m The mass \f$ m \f$ of the particles in the cluster.
     * @param type The type of the particle.
     * @param epsilon The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     */
    Cluster(ParticleContainer &particles, const std::array<double, 3> &x, const std::array<double, 3> &v, double h,
            double m, int type, double epsilon, double sigma);

    /// @brief Destroys the current Cluster.
    virtual ~Cluster();

    /**
     * @brief Gets the position \f$ x \f$ of this Cluster.
     *
     * @return A reference to the position array of this Cluster.
     */
    std::array<double, 3> &getX();

    /**
     * @brief Gets the position \f$ x \f$ of this Cluster (const).
     *
     * @return A const reference to the position array of this Cluster.
     */
    const std::array<double, 3> &getX() const;

    /**
     * @brief Gets the distance \f$ h \f$ between the particles of this Cluster.
     *
     * @return The distance between the particles of the Cluster.
     */
    double getH() const;

    /**
     * @brief Gets the mass \f$ m \f$ of the particles of this Cluster.
     *
     * @return The mass of the particles of the Cluster.
     */
    double getM() const;

    /**
     * @brief Gets the starting velocity \f$ v \f$ of the particles of this Cluster.
     *
     * @return A reference to the velocity array of the Cluster.
     */
    std::array<double, 3> &getV();

    /**
     * @brief Gets the starting velocity \f$ v \f$ of the particles of this Cluster (const).
     *
     * @return A const reference to the velocity array of the Cluster.
     */
    const std::array<double, 3> &getV() const;

    /**
     * @brief Gets the mean velocity for the Maxwell-Boltzmann distribution.
     *
     * @return The mean velocity field of the Cluster.
     */
    double getMeanVelocity() const;

    /**
     * @brief Gets the reference to the central ParticleContainer.
     *
     * @return A reference to the ParticleContainer stored in this container.
     */
    ParticleContainer &getParticles();

    /**
     * @brief Gets the reference to the central ParticleContainer (const).
     *
     * @return A const reference to the ParticleContainer stored in this container.
     */
    const ParticleContainer &getParticles() const;

    /**
     * @brief Initializes the Particle objects in the Cluster.
     *
     * @param dimensions The number of dimensions for which to initialize the particles (default: 2).
     */
    virtual void initialize(size_t dimensions = 2) = 0;
};