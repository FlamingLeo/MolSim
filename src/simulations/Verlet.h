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
#include <list>

class Verlet
{
private:
    std::list<Particle> m_particles; // TODO ParticleContainer...
    double m_start_time;
    double m_end_time;
    double m_delta_t;
    int m_it_freq;

public:
    Verlet(const Arguments &args);
    ~Verlet();

    /**
     * @brief Calculates the force for all particles.
     *
     */
    void calculateF();

    /**
     * @brief Calculates the position for all particles.
     *
     */
    void calculateX();

    /**
     * @brief Calculates the velocity for all particles.
     *
     */
    void calculateV();

    /**
     * @brief Runs the simulation.
     *
     */
    void runSimulation();

    /**
     * @brief Plots the particles.
     *
     * @param iteration
     */
    void plotParticles(int iteration);

    /**
     * @brief Get the Particles object.
     *
     * @return std::list<Particle>
     */
    std::list<Particle> &getParticles();

};