/**
 * @file Thermostat.h
 * @brief Model of a thermostat.
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "ParticleContainer.h"
#include <cmath>

/// @brief Class modeling a thermostat which regulates the temperature of a Particle system.
class Thermostat {
  private:
    /// @brief The dimensions for which the Thermostat temperature regulation should be applied (default: 2).
    int dimension{2};

    /// @brief The kinetic energy \f$ E_{kin} \f$ of the active simulation particles (default: 0).
    double kineticEnergy{0.0};

    /// @brief The Temperature \f$ T \f$ of the system (default: 0).
    double temperature{0.0};

    /// @brief The scaling factor \f$ \beta \f$ by which to scale the Particle velocities (default: 1).
    double scalingFactor{1.0};

    /**
     * @brief The starting temperature \f$ T_{init} \f$ of the system (default: 0).
     *
     * @details Must be specified when initializing a Thermostat.
     */
    double T_init{0.0};

    /**
     * @brief The target temperature \f$ T_{target} \f$ of the system (default: \f$ T_{init} \f$).
     *
     * @details If no target temperature is given (i.e. if the value is set to `INFINITY`), the target temperature is
     * set to \f$ T_{init} \f$.
     */
    double T_target{0.0};

    /**
     * @brief The number of simulation iterations after which to apply the Thermostat functionality (default: 1000).
     *
     * @details Must be specified when initializing a Thermostat.
     */
    int n_thermostat{1000};

    /**
     * @brief The maximum temperature difference \f$ \Delta T \f$ in one Thermostat application (default: \f$ \infty
     * \f$).
     *
     * @details If no temperature difference is specified (i.e. if the value is defaulted to `INFINITY`), the
     * temperature is set directly in one Thermostat application.
     */
    double delta_T{INFINITY};

    /// @brief Determines if the Thermostat should apply gradual velocity scaling or direct velocity scaling (default:
    /// direct).
    bool limitScaling{false};

    /// @brief Determines whether or not to initialize the velocities with Brownian Motion (default: on).
    bool initBrownianMotion{true};

    /// @brief A reference to the Particle system.
    ParticleContainer &particles;

  public:
    /**
     * @brief Construct a new Thermostat with default-initialized values.
     *
     * @param particles The ParticleContainer representing the Particle system.
     */
    explicit Thermostat(ParticleContainer &particles);

    /**
     * @brief Construct a complete Thermostat object with all parameters.
     *
     * @param particles The ParticleContainer representing the Particle system.
     * @param dimension The dimensions for which the Thermostat temperature regulation should be applied.
     * @param T_init The starting temperature \f$ T_{init} \f$ of the system.
     * @param n_thermostat The number of simulation iterations after which to apply the Thermostat functionality.
     * @param T_target The target temperature \f$ T_{target} \f$ of the system.
     * @param delta_T The maximum temperature difference \f$ \Delta T \f$ in one Thermostat application.
     * @param initBrownianMotion Determines whether or not to initialize the velocities with Brownian Motion.
     */
    Thermostat(ParticleContainer &particles, int dimension, double T_init, int n_thermostat, double T_target,
               double delta_T, bool initBrownianMotion);

    /// @brief Destroys the current Thermostat object.
    ~Thermostat();

    /**
     * @brief Initialize the current Thermostat's parameters.
     *
     * @param dimension The dimensions for which the Thermostat temperature regulation should be applied.
     * @param T_init The starting temperature \f$ T_{init} \f$ of the system.
     * @param n_thermostat The number of simulation iterations after which to apply the Thermostat functionality.
     * @param T_target The target temperature \f$ T_{target} \f$ of the system.
     * @param delta_T The maximum temperature difference \f$ \Delta T \f$ in one Thermostat application.
     * @param initBrownianMotion Determines whether or not to initialize the velocities with Brownian Motion.
     */
    void initialize(int dimension, double T_init, int n_thermostat, double T_target, double delta_T,
                    bool initBrownianMotion);

    /**
     * @brief Initialize the Particle velocities with Brownian Motion.
     *
     * @details Assuming a Particle does not have an initial velocity, in order for the system to have a non-zero
     * temperature, the velocity for any given Particle \f$ i \f$ is calculated using a Maxwell-Boltzmann distribution
     * with the factor \f[ f_i = \sqrt{\frac{T_{init}}{m_i}}. \f]
     */
    void initializeBrownianMotion();

    /**
     * @brief Calculates the kinetic energy \f$ E_{kin} \f$ for all Particle objects in the system.
     *
     * @details The kinetic energy is calculated using the formula \f[ E_{kin} = \sum_{i=1}^N \frac{m_i \langle v_i, v_i
     * \rangle}{2} \f] where \f$ N \f$ denotes the total number of active Particle objects.
     */
    void calculateKineticEnergy();

    /**
     * @brief Calculates the current temperature \f$ T \f$ of the system.
     *
     * @details The temperature is calculated after getting the kinetic energy \f$ E_{kin} \f$ of all Particle objects
     * in the system by rearranging the equation \f[ E_{kin} = \frac{D \cdot N}{2}k_B T \f] and solving for \f$ T \f$,
     * with \f$ N \f$ being the total number of active Particle objects, \f$ D \f$ being the number of dimensions and
     * \f$ k_B = 1 \f$.
     */
    void calculateTemp();

    /**
     * @brief Calculates the scaling factor \f$ \beta \f$.
     *
     * @details The scaling factor is calculated using the formula \f[ \beta = \sqrt{\frac{T_{new}}{T_{current}}} \f]
     * where \f$ T_{new} \f$ is either the target temperature if no limit \f$ \Delta T \f$ is given or if the target
     * temperature can be reached in the next thermostat application, or \f$ T_{current} \pm \Delta T \f$ otherwise
     * (depending if the current temperature is above or below the target temperature).
     */
    void calculateScalingFactor();

    /**
     * @brief Updates the system temperature.
     *
     * @param currentStep The current iteration of the simulation, used to determine whether or not to apply the
     * thermostat. In the first iteration, if `initBrownianMotion` is set, the Particle velocities will be initialized
     * with Brownian Motion. Otherwise, nothing is done in the first iteration.
     */
    void updateSystemTemp(int currentStep);

    /// @brief Gets the kinetic energy \f$ E_{kin} \f$ of the active simulation particles.
    double getKineticEnergy() const;

    /// @brief Gets the current temperature \f$ T \f$ of the system.
    double getTemp() const;

    /// @brief Gets the starting temperature \f$ T_{init} \f$ of the system.
    double getInitTemp() const;

    /// @brief Gets the target temperature \f$ T_{target} \f$ of the system.
    double getTargetTemp() const;

    /// @brief Gets the maximum temperature difference \f$ \Delta T \f$ in one Thermostat application.
    double getDeltaT() const;

    /// @brief Gets the scaling factor \f$ \beta \f$ by which to scale the Particle velocities.
    double getScalingFactor() const;

    /// @brief Gets the number of simulation iterations after which to apply the Thermostat functionality.
    int getTimestep() const;

    /// @brief Gets a reference to the Particle system.
    ParticleContainer &getParticles() const;
};