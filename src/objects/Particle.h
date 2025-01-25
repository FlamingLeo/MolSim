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

#include "utils/OMPWrapper.h"
#include <array>
#include <string>
#include <vector>

#define TYPE_DEFAULT 0
#define SIGMA_DEFAULT 1
#define EPSILON_DEFAULT 5
#define K_DEFAULT 0
#define R0_DEFAULT 0
#define FZUP_DEFAULT 0
#define MASS_ERROR "The mass of a particle must be positive for the currently available simulations!"

#ifdef NOUTFLOW
#define CONTINUE_IF_INACTIVE(p) (void)0
#else
#define CONTINUE_IF_INACTIVE(p)                                                                                        \
    if (!(p).isActive())                                                                                               \
        continue;
#endif

/// @brief Particle class modeling a particle's position, velocity, force, mass and type.
class Particle {
  private:
    /// @brief Position \f$ x \f$ of the particle.
    std::array<double, 3> x;

    /// @brief Velocity \f$ v \f$ of the particle.
    std::array<double, 3> v;

    /// @brief Thermal motion of the particle
    std::array<double, 3> thermal_motion;

    /// @brief Force \f$ F \f$ effective on this particle.
    std::array<double, 3> f;

    /// @brief Force \f$ F_\text{old} \f$ which was effective on this particle.
    std::array<double, 3> old_f;

    /// @brief Direct (up-down, left-right) neighbours of the particle (for membranes).
    std::vector<std::reference_wrapper<Particle>> direct_neighbours;

    /// @brief Diagonal neighbours of the particle (for membranes).
    std::vector<std::reference_wrapper<Particle>> diagonal_neighbours;

    /// @brief Mass \f$ m \f$ of this particle.
    double m;

    /// @brief Type of the particle. 1 if wall, 0 otherwise
    int type; // this could probably be made an enum now

    /// @brief Depth \f$ \epsilon \f$ of the potential well. Lennard-Jones parameter.
    double epsilon;

    /// @brief Distance \f$ \sigma \f$ where the Lennard-Jones potential reaches zero. Lennard-Jones parameter.
    double sigma;

    /// @brief The stiffness constant \f$ k \f$ for membrane simulations.
    double k;

    /// @brief The average bond length \f$ r_0 \f$ for membrane simulations.
    double r_0;

    /// @brief The constant upward force \f$ F_{Z-UP} \f$ for membrane simulations.
    double fzup;

    /// @brief The cell index of the particle, to be used with the linked cell method.
    int cellIndex;

    /// @brief A mutual exclusion lock, used alongside parallelization.
    omp_lock_t lock;

    /**
     * @brief The status of the particle.
     *
     * @details If the particle is marked "active", it will be considered in the time integration calculation.
     * Otherwise, it is ignored.
     *
     */
    bool active{true};

    /// @brief A unique ID for this particle. Currently only used for debug purposes.
    int id;

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
     * @brief Construct a new Particle object using explicit values for each new data field.
     *
     * Given that the current simulations make use of the particle mass as the divisor in some formulas, the mass may
     * not be negative or 0.
     *
     * @param x A reference to the array containing data for the position \f$ x \f$.
     * @param v A reference to the array containing data for the velocity \f$ v \f$.
     * @param m The mass \f$ m \f$ of the particle.
     * @param type The type of the particle. (optional)
     * @param eps The Lennard-Jones parameter \f$ \epsilon \f$ of the particle. (optional)
     * @param sigma The Lennard-Jones parameter \f$ \sigma \f$ of the particle. (optional)
     * @param k The stiffness constant \f$ k \f$, used for membrane simulations. (optional)
     * @param r_0 The average bond length \f$ r_0 \f$, used for membrane simulations. (optional)
     * @param fzup The constant upward force \f$ F_{Z-UP} \f$, used for membrane simulations. (optional)
     */
    Particle(const std::array<double, 3> &x, const std::array<double, 3> &v, double m, int type = TYPE_DEFAULT,
             double eps = EPSILON_DEFAULT, double sigma = SIGMA_DEFAULT, double k = K_DEFAULT, double r_0 = R0_DEFAULT,
             double fzup = FZUP_DEFAULT);

    /**
     * @brief Construct a new Particle object using explicit values for every data field.
     *
     * There are no default parameters here; every value must be initialized.
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
    Particle(const std::array<double, 3> &x, const std::array<double, 3> &v, const std::array<double, 3> &f,
             const std::array<double, 3> &old_f, double m, int type, double eps, double sigma, double k, double r_0,
             double fzup, int cellIndex);

    /// @brief Destroys the Particle object.
    virtual ~Particle();

    /**
     * @brief Gets the position \f$ x \f$ of this particle.
     *
     * @return A reference to the position array of this particle.
     */
    inline std::array<double, 3> &getX() { return x; }

    /**
     * @brief Gets the velocity \f$ v \f$ of this particle.
     *
     * @return A reference to the velocity array of this particle.
     */
    inline std::array<double, 3> &getV() { return v; }

    /**
     * @brief Gets the force \f$ F \f$ effective on this particle.
     *
     * @return A reference to the force array of this particle.
     */
    inline std::array<double, 3> &getF() { return f; }

    /**
     * @brief Gets the force \f$ F_\text{old} \f$ previously effective on this particle.
     *
     * @return A reference to the old force array of this particle.
     */
    inline std::array<double, 3> &getOldF() { return old_f; }

    /**
     * @brief Gets the direct neighbours of this particle.
     *
     * @return A reference to the vector of direct neighbour particle references of this particle.
     */
    inline std::vector<std::reference_wrapper<Particle>> &getDirectNeighbours() { return direct_neighbours; }

    /**
     * @brief Gets the direct neighbours of this particle (const).
     *
     * @return A const reference to the vector of direct neighbour particle references of this particle.
     */
    inline const std::vector<std::reference_wrapper<Particle>> &getDirectNeighbours() const {
        return direct_neighbours;
    }

    /**
     * @brief Gets the diagonal neighbours of this particle.
     *
     * @return A reference to the vector of diagonal neighbour particle references of this particle.
     */
    inline std::vector<std::reference_wrapper<Particle>> &getDiagonalNeighbours() { return diagonal_neighbours; }

    /**
     * @brief Gets the diagonal neighbours of this particle (const).
     *
     * @return A const reference to the vector of diagonal neighbour particle references of this particle.
     */
    inline const std::vector<std::reference_wrapper<Particle>> &getDiagonalNeighbours() const {
        return diagonal_neighbours;
    }

    /**
     * @brief Gets the position \f$ x \f$ of this particle (const).
     *
     * @return A const reference to the position array of this particle.
     */
    inline const std::array<double, 3> &getX() const { return x; }

    /**
     * @brief Gets the velocity \f$ v \f$ of this particle (const).
     *
     * @return A const reference to the velocity array of this particle.
     */
    inline const std::array<double, 3> &getV() const { return v; }

    /**
     * @brief Gets the thermal motion \f$ thermal_motion \f$ of this particle (const).
     *
     * @return A const reference to the thermal motion array of this particle.
     */
    inline const std::array<double, 3> &getThermalMotion() const { return thermal_motion; }

    /**
     * @brief Gets the force \f$ F \f$ effective on this particle (const).
     *
     * @return A const reference to the force array of this particle.
     */
    inline const std::array<double, 3> &getF() const { return f; }

    /**
     * @brief Gets the force \f$ F_\text{old} \f$ previously effective on this particle (const).
     *
     * @return A const reference to the old force array of this particle.
     */
    inline const std::array<double, 3> &getOldF() const { return old_f; }

    /**
     * @brief Gets the mass \f$ m \f$ of the particle.
     *
     * @return The mass of the particle.
     */
    inline double getM() const { return m; }

    /**
     * @brief Gets the type of the particle.
     *
     * @return The type of the particle.
     */
    inline int getType() const { return type; }

    /**
     * @brief Gets the Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     *
     * @return The Lennard-Jones parameter \f$ \epsilon \f$ of the particle.
     */
    inline double getEpsilon() const { return epsilon; }

    /**
     * @brief Gets the Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     *
     * @return The Lennard-Jones parameter \f$ \sigma \f$ of the particle.
     */
    inline double getSigma() const { return sigma; }

    /**
     * @brief Gets the stiffness constant \f$ k \f$.
     *
     * @return The stiffness constant \f$ k \f$.
     */
    inline double getK() const { return k; }

    /**
     * @brief Gets the average bond length of a molecule pair \f$ r_0 \f$.
     *
     * @return The average bond length \f$ r_0 \f$.
     */
    inline double getR0() const { return r_0; }

    /**
     * @brief Gets the constant upward force \f$ F_{Z-UP} \f$.
     *
     * @return The constant upward force \f$ F_{Z-UP} \f$.
     */
    inline double getFZUP() const { return fzup; }

    /**
     * @brief Get the index of the particle in a CellContainer.
     *
     * For use with the linked cell method.
     *
     * @return The index of the particle in a grid of cells.
     */
    inline int getCellIndex() const { return cellIndex; }

    /**
     * @brief Checks if the particle is currently active.
     *
     * @return true if the particle is currently active.
     * @return false if the particle is currently inactive.
     */
    inline bool isActive() const { return active; }

    /**
     * @brief Get the ID of this particle.
     *
     * Currently only used for debugging purposes.
     *
     * @return The ID of this particle.
     */
    inline int getId() const { return id; }

    /**
     * @brief Get a reference to the Lock object.
     *
     * @return A reference to the Lock object.
     */
    inline omp_lock_t &getLock() { return lock; }

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
     * @brief Sets the direct neighbours of the particle.
     *
     * @param neighbours A reference to the vector of references to the neighbours.
     */
    void setDirectNeighbours(const std::vector<std::reference_wrapper<Particle>> &neighbours);

    /**
     * @brief Sets the diagonal neighbours of the particle.
     *
     * @param neighbours A reference to the vector of references to the neighbours.
     */
    void setDiagonalNeighbours(const std::vector<std::reference_wrapper<Particle>> &neighbours);

    /**
     * @brief Sets the new thermal motion \f$ thermal_motion \f$ of the particle to a given value.
     *
     * @param new_thermal_motion A reference to the array containing the new thermal motion of this particle.
     */
    void setThermalMotion(const std::array<double, 3> &new_thermal_motion);

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
     * @brief Sets the new stiffness constant \f$ k \f$ to a given value.
     *
     * @param new_sigma The new stiffness constant \f$ k \f$.
     */
    void setK(double new_k);

    /**
     * @brief Sets the new upward force \f$ F_{Z-UP} \f$ to a given value.
     *
     * @param new_sigma The new upward force \f$ F_{Z-UP} \f$.
     */
    void setFZUP(double new_fzup);

    /**
     * @brief Sets the new average bond length \f$ r_0 \f$ to a given value.
     *
     * @param new_sigma The new average bond length \f$ r_0 \f$.
     */
    void setR0(double new_r_0);

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
