/**
 * @file Timer.h
 * @brief Header-only class for timing code execution.
 * @date 2024-12-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "CLIUtils.h"
#include <chrono>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

#ifdef DO_BENCHMARKING
#define TIMER_RESET(_x) _x.reset()
#define TIMER_PRINT_ELAPSED(_x) _x.printElapsed()
#define TIMER_PRINT_MUPS(_x) _x.printMUPS()
#define TIMER_UPDATE_MOLECULES(_x, _y) _x.incrementMoleculeUpdates(_y)
#else
#define TIMER_RESET(_x) (void)0
#define TIMER_PRINT_ELAPSED(_x) (void)0
#define TIMER_PRINT_MUPS(_x) (void)0
#define TIMER_UPDATE_MOLECULES(_x, _y) (void)0
#endif

/// @brief Class used to measure runtime performance and log molecule updates per second (MUPS).
class Timer {
  private:
    /// @brief The start time of the timing process.
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    /// @brief The total number of molecule updates in the given timeframe.
    long m_moleculeUpdates{0};

  public:
    /// @brief Constructs a new Timer.
    Timer() {
        SPDLOG_TRACE("Created a new blank timer... you shouldn't be seeing this if you want to measure runtime!");
    }

    /// @brief Resets the start time.
    inline void reset() { m_start = std::chrono::high_resolution_clock::now(); }

    /**
     * @brief Gets the elapsed time since the start.
     *
     * @tparam T The unit of measurement to be returned (e.g. milliseconds, seconds...).
     * @return The time passed since the stored start time.
     */
    template <typename T> inline auto elapsed() const {
        return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - m_start).count();
    }

    /**
     * @brief Increments the total number of molecule updates by a given amount.
     *
     * @param summand The amount to add to the molecule update counter.
     */
    inline void incrementMoleculeUpdates(long summand) { m_moleculeUpdates += summand; }

    /// @brief Prints the elapsed time.
    inline void printElapsed() const {
        auto elapsedInMillis = elapsed<std::chrono::milliseconds>();
        auto elapsedInSecs = elapsedInMillis * 0.001;
        SPDLOG_INFO("Elapsed: {}ms ({}s)", elapsedInMillis, elapsedInSecs);
    }

    /// @brief Prints the molecule updates per second.
    inline void printMUPS() const {
        auto elapsedInSec = (elapsed<std::chrono::milliseconds>() * 0.001);
        SPDLOG_INFO("Molecule Updates per Second (MUPS): {}", static_cast<double>(m_moleculeUpdates) / elapsedInSec);
    }
};