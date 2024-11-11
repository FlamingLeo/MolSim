/**
 * @file Verlet.h
 * @brief Simulation of the Velocity-Störmer-Verlet method.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "Simulation.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <string>

/// @brief Class containing the functionality and parameters required to run a simulation of the Velocity-Störmer-Verlet
/// method.
class Verlet : public Simulation {
  private:
    /// @brief The ParticleContainer containing the particles used in the simulation.
    ParticleContainer m_particles;
    /// @brief The start time \f$ t_0 \f$ of the simulation.
    double m_startTime; // default: 0
    /// @brief The end time \f$ t_\text{end} \f$ of the simulation.
    double m_endTime; // default: 1000
    /// @brief The timestep \f$ \Delta t \f$ used during the simulation.
    double m_delta_t; // default: 0.014
    /// @brief The output (logging) frequency for generating a file output.
    int m_itFreq;
    /// @brief The type of the generated output file.
    WriterType m_type;

  public:
    /**
     * @brief Initialize a new Verlet simulation with specific particles and the given program arguments.
     *
     * @param m_particles A ParticleContainer containing the particles used in the simulation.
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    Verlet(const ParticleContainer &m_particles, const Arguments &args);

    /**
     * @brief Initialize a new Verlet simulation from a specified file name using the given program arguments.
     *
     * @param filename The path to the file containing the particle information.
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    Verlet(const std::string &filename, const Arguments &args);

    /**
     * @brief Initialize a new Verlet simulation with the given program arguments.
     *
     * @param args An Arguments struct containing relevant simulation parameters.
     */
    explicit Verlet(const Arguments &args);

    /// @brief Default destructor destroying data.
    ~Verlet();

    /**
     * @brief Calculates the force \f$ F \f$ for all particles using a naive approach.
     *
     * @details For each particle in the simulation, the method works by first calculating the effective force \f[
     * F_{ij} = \frac{m_i m_j}{(|| x_i - x_j ||_2)^3} (x_j - x_i) \f] between each pair of particles \f$ i \f$ and \f$ j
     * \f$. Afterwards, the effective force for a single particle \f$ i \f$ is calculated using the formula \f[ F_i =
     * \sum_{j=1, j \neq i}^p F_{ij} \f] where \f$ p \f$ denotes the total number of particles.
     *
     */
    void calculateF();

    /**
     * @brief Calculates the force \f$ F \f$ for all particles using Newton's third law of motion.
     *
     * @details <em> For every action, there is an equal and opposite reaction.</em>
     *
     * Instead of looping through and calculating the force for each individual particle, we avoid recalculating force
     * for pairs \f$(p_i, p_j)\f$ which have already been computed and apply the reciprocal forces to both particles
     * simultaneously using Newton's third law: \f[ F_{ij} = -F_{ji}. \f]
     *
     */
    void calculateF_thirdLaw();

    /**
     * @brief Calculates the position \f$ x \f$ for all particles.
     *
     * @details The position \f$ x_i \f$ of a given particle \f$ i \f$ at the next unit of time \f$ t_{n+1} \f$ is
     * calculated using the formula \f[ x_i(t_{n+1}) = x_i(t_n) + \Delta t \cdot v_i(t_n) + (\Delta t)^2
     * \frac{F_i(t_n)}{2m_i}. \f]
     */
    void calculateX();

    /**
     * @brief Calculates the velocity \f$ v \f$ for all particles.
     *
     * @details The velocity \f$ v_i \f$ of a given particle \f$ i \f$ at the next unit of time \f$ t_{n+1} \f$ is
     * calculated using the formula \f[ v_i(t_{n+1}) = v_i(t_n) + \Delta t \frac{F_i(t_n) + F_i(t_{n+1})}{2m_i}. \f]
     */
    void calculateV();

    /// @brief Runs the simulation.
    void runSimulation() override;

    /// @brief Gets a reference to the ParticleContainer storing the simulation's particles.
    /// @return A reference to the simulation's ParticleContainer.
    ParticleContainer &getParticles();
};