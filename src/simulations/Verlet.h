/**
 * @file Verlet.h
 * @brief Simulation of the Velocity-Störmer-Verlet method.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "objects/Particle.h"
#include "utils/Arguments.h"
#include "Simulation.h"
#include <list>

/// @brief Class containing the functionality and parameters required to run a simulation of the Velocity-Störmer-Verlet method.
class Verlet : public Simulation
{
private:
    /// @brief The list of particles used in the simulation.
    std::list<Particle> m_particles; // TODO ParticleContainer...
    /// @brief The start time of the simulation.
    double m_start_time;
    /// @brief The end time of the simulation.
    double m_end_time;
    /// @brief The timestep used during the simulation.
    double m_delta_t;
    /// @brief The output (logging) frequency for generating a file output.
    int m_it_freq;

public:
    Verlet(const Arguments &args);
    ~Verlet();

    /// @brief Calculates the force for all particles.
    void calculateF();

    /// @brief Calculates the position for all particles.
    void calculateX();

    /// @brief Calculates the velocity for all particles.
    void calculateV();

    /// @brief Runs the simulation.
    void runSimulation() override;

    std::list<Particle> &getParticles();
};