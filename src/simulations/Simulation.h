/**
 * @file Simulation.h
 * @brief Class for initializing and running a basic simulation.
 * @date 2024-10-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "io/output/FileWriter.h"
#include "io/output/WriterFactory.h"
#include "io/output/XMLWriter.h"
#include "objects/FlowSimulationAnalyzer.h"
#include "objects/ParticleContainer.h"
#include "objects/Thermostat.h"
#include "strategies/StrategyFactory.h"
#include "utils/Arguments.h"
#include "utils/Timer.h"
#include <cassert>
#include <memory>
#include <optional>
#include <string>

#if (!defined(DO_BENCHMARKING) && !defined(DO_PROFILING))
#define SIM_SERIALIZE_XML(_a, _b, _c, _d)                                                                              \
    do {                                                                                                               \
        XMLWriter xmlw{_a};                                                                                            \
        xmlw.serialize(_b, _c, _d);                                                                                    \
    } while (0)
#define SIM_INIT_WRITER(_a, _b, _c) _a = WriterFactory::createWriter(_b, _c)
#define SIM_WRITE_OUTPUT(_a, _b, _c, _d, _e)                                                                           \
    do {                                                                                                               \
        if (_a % _b == 0) {                                                                                            \
            _c->writeParticles(_d, _a, _e);                                                                            \
        }                                                                                                              \
    } while (0)
#else
#define SIM_SERIALIZE_XML(_a, _b, _c, _d) (void)0
#define SIM_INIT_WRITER(_a, _b, _c) (void)0
#define SIM_WRITE_OUTPUT(_a, _b, _c, _d, _e) (void)0
#endif

#ifdef NOUTFLOW
#define CHECK_NOUTFLOW(_x, _y)                                                                                         \
    if (std::any_of(_x._y.begin(), _x._y.end(),                                                                        \
                    [](BoundaryCondition condition) { return condition == BoundaryCondition::OUTFLOW; })) {            \
        CLIUtils::error("Cannot run simulation with outflow conditions when NOUTFLOW is defined! Please recompile "    \
                        "the program without defining it.");                                                           \
    }
#else
#define CHECK_NOUTFLOW(_x, _y) (void)0
#endif

/// @brief Class defining a time-integration simulation.
class Simulation {
  protected:
    /// @brief Reference to the ParticleContainer containing the particles used in the simulation.
    ParticleContainer &m_particles;

    /// @brief Reference to the Arguments struct containing simulation parameters.
    Arguments &m_args;

    /// @brief Reference to the Thermostat, for temperature regulation.
    Thermostat &m_thermostat;

    /// @brief Reference to the Anlayzer, for statistics computation.
    FlowSimulationAnalyzer &m_analyzer;

    /// @brief Default Anlayzer when none is needed.
    static FlowSimulationAnalyzer defaultAnalyzer;

    /// @brief The total number of iterations for which the simulation will run.
    int m_totalIt;

    /// @brief A pointer to the FileWriter object used for writing file output.
    std::unique_ptr<FileWriter> m_writer;

    /// @brief Function for calculating the Particle velocities.
    TimeIntegrationFuncs::VFunc m_calculateV;

    /// @brief Function for calculating the Particle positions.
    TimeIntegrationFuncs::XFunc m_calculateX;

    /// @brief Function for calculating the force effective on each Particle.
    StrategyFactory::FFunc m_calculateF;

    /// @brief Timer used to measure simulation runtime and log molecule updates per second (MUPS).
    Timer m_timer;

    /// @brief Base function for initializing Simulation parameters.
    void initializeBase();

    /**
     * @brief Runs a basic simulation loop.
     *
     * @details The particle container must not be empty.
     *
     * @param lc An optional parameter for linked cell algorithms.
     */
    void runSimulationLoop(CellContainer *lc = nullptr);

  public:
    /**
     * @brief Constructs a new Simulation.
     *
     * @param pc The ParticleContainer containing the simulation molecules.
     * @param args The Arguments struct containing the simulation metadata.
     * @param t The Thermostat used for temperature regulation.
     * @param analyzer The Anlayzer used for statistics computation.
     */
    Simulation(ParticleContainer &pc, Arguments &args, Thermostat &t, FlowSimulationAnalyzer &analyzer);

    /// @brief Destroys the current Simulation object.
    virtual ~Simulation();

    /// @brief Main function for running the simulation.
    virtual void runSimulation();

    /**
     * @brief Gets a reference to the ParticleContainer storing the simulation's particles.
     *
     * @return A reference to the simulation's ParticleContainer.
     */
    ParticleContainer &getParticles();
};