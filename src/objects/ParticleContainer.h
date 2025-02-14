/**
 * @file ParticleContainer.h
 * @brief Class for encapsulating and iterating over multiple Particle objects.
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/Particle.h"
#include <array>
#include <string>
#include <utility>
#include <vector>

/// @brief Class for encapsulating multiple Particle objects..
class ParticleContainer {
    /// @brief The chosen data type for storing Particle objects dynamically and contiguously is a std::vector.
    using ContainerType = std::vector<Particle>;

  private:
    /// @brief A ContainerType storing multiple Particle objects, forming the base of this class.
    ContainerType m_particles;

    /// @brief The number of iterations after which the special upward force will no longer be applied, for membrane
    /// simulations.
    int m_specialForceLimit;

    /* iterator definitions */
  public:
    /// @brief Standard library iterator function for marking the beginning of the iteration process.
    /// @return An iterator pointing to the first element of m_particles.
    ContainerType::iterator begin();

    /// @brief Standard library iterator function for marking the end of the iteration process.
    /// @return An iterator pointing to the last element of m_particles.
    ContainerType::iterator end();

    /// @brief Standard library iterator function for marking the beginning of the iteration process of a const
    /// ParticleContainer.
    /// @return An iterator pointing to the first element of m_particles.
    ContainerType::const_iterator begin() const;

    /// @brief Standard library iterator function for marking the end of the iteration process of a const
    /// ParticleContainer.
    /// @return An iterator pointing to the last element of m_particles.
    ContainerType::const_iterator end() const;

    /// @brief Custom pair iterator for iterating through all possible pairs of Particle objects.
    class PairIterator {
      private:
        /// @brief A standard library iterator for iterating through the outer pairs.
        ContainerType::iterator outer_it;
        /// @brief A standard library iterator for iterating through the inner pairs.
        ContainerType::iterator inner_it;
        /// @brief A standard library iterator marking the beginning of the iteration.
        ContainerType::iterator container_begin;
        /// @brief A standard library iterator marking the end of the iteration.
        ContainerType::iterator container_end;

      public:
        /**
         * @brief Construct a new Pair Iterator.
         *
         * @param outer A standard library iterator for iterating through the outer pairs.
         * @param inner A standard library iterator for iterating through the inner pairs.
         * @param begin A standard library iterator marking the beginning of the iteration.
         * @param end A standard library iterator marking the end of the iteration.
         */
        PairIterator(ContainerType::iterator outer, ContainerType::iterator inner, ContainerType::iterator begin,
                     ContainerType::iterator end);

        /**
         * @brief Override of the * operator, returning a pair of Particles.
         *
         * @return A pair of Particle references.
         */
        std::pair<Particle &, Particle &> operator*() const;

        /**
         * @brief Override of the ++ operator.
         *
         * @details Begins by incrementing the inner iterator. Once the inner iterator has reached the final Particle in
         * the container, the outer iterator gets incremented and the inner iterator is reset to the beginning.
         *
         * @return A PairIterator& used to continue iteration.
         */
        PairIterator &operator++();

        /**
         * @brief Checks if two iterator instances point to the same Particle pair.
         *
         * @param other The other pair iterator to compare iteration progress with.
         * @return true if both iterators point to the same object.
         * @return false if at least one iterator points to a different object.
         */
        bool operator==(const PairIterator &other) const;

        /**
         * @brief Checks if two iterator instances do not point to the same Particle pair.
         *
         * @param other The other pair iterator to compare iteration progress with.
         * @return true if at least one iterator points to a different object.
         * @return false if both iterators point to the same object.
         */
        bool operator!=(const PairIterator &other) const;
    };

    /**
     * @brief Override of the [] operator.
     *
     * Gets a Particle by index. Does NOT perform bounds checking.
     *
     * @param index The index of the Particle to get.
     * @return A reference to the Particle at the specified index.
     */
    Particle &operator[](size_t index);

    /**
     * @brief Override of the [] operator (const).
     *
     * Gets a Particle by index. Does NOT perform bounds checking.
     *
     * @param index The index of the Particle to get.
     * @return A const reference to the Particle at the specified index.
     */
    const Particle &operator[](size_t index) const;

    /**
     * @brief Function marking the beginning of the pair iteration process.
     *
     * @return A PairIterator pointing to the first possible particle pair.
     */
    PairIterator beginPairs();

    /**
     * @brief Function marking the end of the pair iteration process.
     *
     * @return A PairIterator pointing to the last possible particle pair.
     */
    PairIterator endPairs();

    /* container functions */
    /// @brief Constructs a ParticleContainer with an empty default-initialized ContainerType.
    ParticleContainer();

    /// @brief Constructs a ParticleContainer with an empty ContainerType and reserves space for num_particle Particle
    /// entries.
    /// @param num_particles The amount of space to be reserved in the empty ContainerType.
    explicit ParticleContainer(size_t num_particles);

    /// @brief Destroys the ParticleContainer object.
    virtual ~ParticleContainer();

    /**
     * @brief Adds an already existing Particle to the container.
     *
     * @param particle The Particle to be added to m_particles.
     */
    void addParticle(const Particle &particle);

    /**
     * @brief Creates and adds a new particle to the container.
     *
     * @param x The position of the new particle.
     * @param v The velocity of the new particle.
     * @param m The mass of the new particle.
     * @param type The type of the new particle.
     * @param eps The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     * @param k The stiffness constant \f$ k \f$, used for membrane simulations.
     * @param r_0 The average bond length \f$ r_0 \f$, used for membrane simulations.
     * @param fzup The constant upward force \f$ F_{Z-UP} \f$, used for membrane simulations.
     */
    void addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m, int type = TYPE_DEFAULT,
                     double eps = EPSILON_DEFAULT, double sigma = SIGMA_DEFAULT, double k = K_DEFAULT,
                     double r_0 = R0_DEFAULT, double fzup = FZUP_DEFAULT);

    /**
     * @brief Creates and adds a new complete particle to the container.
     *
     * @param x A reference to the array containing data for the position \f$ x \f$.
     * @param v A reference to the array containing data for the velocity \f$ v \f$.
     * @param f A reference to the array containing data for the force \f$ F \f$ effective on this particle.
     * @param old_f A reference to the array containing data for the old force \f$ F \f$ effective on this particle.
     * @param m The mass \f$ m \f$ of the particle.
     * @param type The type of the particle.
     * @param eps The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     * @param k The stiffness constant \f$ k \f$, used for membrane simulations.
     * @param r_0 The average bond length \f$ r_0 \f$, used for membrane simulations.
     * @param fzup The constant upward force \f$ F_{Z-UP} \f$, used for membrane simulations.

     * @param cellIndex The index of this particle inside a cell. For use with the linked cell method.
     */
    void addParticle(const std::array<double, 3> &x, const std::array<double, 3> &v, const std::array<double, 3> &f,
                     const std::array<double, 3> &old_f, double m, int type, double eps, double sigma, double k,
                     double r_0, double fzup, int cellIndex);

    /**
     * @brief Reserves a certain amount of spaces inside the Particle vector.
     *
     * @param capacity The amount of contiguous spaces to reserve in m_particles.
     */
    void reserve(size_t capacity);

    /**
     * @brief Gets a Particle by index. Performs bounds checking and terminates on invalid index.
     *
     * @param index The index of the Particle to get.
     * @return A reference to the Particle at the specified index.
     */
    Particle &get(size_t index);

    /**
     * @brief Gets a const Particle by index. Performs bounds checking and terminates on invalid index.
     *
     * @param index The index of the Particle to get.
     * @return A const reference to the Particle at the specified index.
     */
    const Particle &get(size_t index) const;

    /**
     * @brief Gets the number of iterations after which the special upward force will no longer be applied.
     *
     * @return The number of iterations after which the special upward force will no longer be applied.
     */
    int getSpecialForceLimit() const;

    /**
     * @brief Sets the number of iterations after which the special upward force will no longer be applied.
     *
     * @param limit The number of iterations after which the special upward force will no longer be applied.
     */
    void setSpecialForceLimit(int limit);

    /// @brief Decrements the number of iterations after which the special upward force will no longer be applied by
    /// one.
    void decrementSpecialForceLimit();

    /**
     * @brief Returns the size of the container.
     *
     * @return The size of the container.
     */
    size_t size() const;

    /**
     * @brief Returns the amount of active particles in the container.
     *
     * @return The number of active particles in the container.
     */
    size_t activeSize() const;

    /**
     * @brief Returns the amount of active, mobile, non-wall particles in the container.
     *
     * @return The number of active, mobile, non-wall particles in the container.
     */
    size_t nonWallSize() const;

    /**
     * @brief Checks if the container is empty.
     *
     * @return true if there are no Particle objects in the container.
     * @return false if there is at least one Particle objects in the container.
     */
    bool isEmpty() const;

    /**
     * @brief Gets a reference to the inner Particle container.
     *
     * @return A ContainerType& referencing m_particles.
     */
    ContainerType &getParticles();

    /**
     * @brief Overload of the equality operator. Checks if two ParticleContainers have the same Particle objects.
     *
     * @param other The ParticleContainer to compare the current one with.
     * @return true if both ParticleContainers contain the same particles.
     * @return false if there is at least one differing particle in one of the containers or the sizes differ.
     */
    bool operator==(const ParticleContainer &other) const;

    /**
     * @brief Overload of the inequality operator. Checks if two ParticleContainers have the at least one differing
     * Particle object or different sizes.
     *
     * @param other The ParticleContainer to compare the current one with.
     * @return true if there is at least one differing particle in one of the containers or the sizes differ.
     * @return false if both ParticleContainers contain the same particles.
     */
    bool operator!=(const ParticleContainer &other) const;

    /**
     * @brief Returns a string representation of this container.
     *
     * @return A std::string of this container.
     */
    std::string toString() const;
};