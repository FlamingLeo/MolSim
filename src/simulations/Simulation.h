/**
 * @file Simulation.h
 * @brief Interface for Simulation classes.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "io/output/FileWriter.h"
#include "io/output/WriterFactory.h"
#include "objects/ParticleContainer.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include <cassert>
#include <memory>
#include <string>

/// @brief Abstract class defining a possible simulation.
class Simulation {
  protected:
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

    /// @brief The total number of iterations for which the simulation will run.
    int m_totalIt;

    /// @brief The type of the generated output file.
    WriterType m_type;

    /// @brief The basename of the generated files.
    std::string m_basename;

    /// @brief A pointer to the FileWriter object used for writing file output.
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating the Particle velocities.
    TimeIntegrationFuncs::VFunc m_calculateV;

    /// @brief Function for calculating the Particle positions.
    TimeIntegrationFuncs::XFunc m_calculateX;

    /// @brief Function for calculating the force effective on each Particle.
    StrategyFactory::FFunc m_calculateF;

    /**
     * @brief Base function for initializing Simulation parameters.
     *
     * @param args The Arguments struct to read parameters from.
     * @param type Variable used for deciding whether a naive or optimized approach is used for calculating forces of
     * particles.
     * @param simType The Simulation type, i.e. a subclass which inherits from Simulation.
     */
    inline void initializeBase(const Arguments &args, int type, SimulationType simType) {
        m_startTime = args.startTime;
        m_endTime = args.endTime;
        m_delta_t = args.delta_t;
        m_itFreq = args.itFreq;
        m_totalIt = static_cast<int>((args.endTime - args.startTime) / args.delta_t);
        m_type = args.type;
        m_basename = args.basename;

        m_writer = WriterFactory::createWriter(m_type, m_basename);
        auto [cvx, cf] = StrategyFactory::getSimulationFunctions(simType, type);
        m_calculateX = cvx.xf;
        m_calculateV = cvx.vf;
        m_calculateF = cf;
    }

    /**
     * @brief Runs a basic simulation loop.
     *
     * The particle container must not be empty.
     *
     * @param epsilon An optional parameter based on the Simulation type (currently, only LJ is supported).
     * @param sigma An optional parameter based on the Simulation type (currently, only LJ is supported).
     * @param lc An optional parameter for linked cell algorithms.
     * @param cutoff An optional parameter for simulations that use a cutoff radius.
     */
    inline void runSimulationLoop(double epsilon = 0, double sigma = 0, CellContainer *lc = nullptr,
                                  double cutoff = 1) {
        // bandaid fix to ensure working with the correct container when using the linked cell method
        ParticleContainer &container = (lc ? lc->getParticles() : m_particles);

        assert(!(container.isEmpty()) && "Cannot run simulation without particles!");

        double currentTime = m_startTime;
        int iteration = 0;

        while (currentTime < m_endTime) {
            m_calculateX(container, m_delta_t, lc);
            m_calculateF(container, epsilon, sigma, cutoff, lc);
            m_calculateV(container, m_delta_t);

            iteration++;

#ifndef DO_BENCHMARKING
            if (iteration % m_itFreq == 0) {
                m_writer->writeParticles(container, iteration, m_totalIt);
            }
#endif
            currentTime += m_delta_t;
        }
    }

  public:
    /// @brief Interface function for running the simulation.
    virtual void runSimulation() = 0;

    /// @brief Destroys the current Simulation object.
    virtual ~Simulation() {}

    /**
     * @brief Gets a reference to the ParticleContainer storing the simulation's particles.
     *
     * @return A reference to the simulation's ParticleContainer.
     */
    inline ParticleContainer &getParticles() { return m_particles; }
};