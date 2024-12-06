#ifndef NDEBUG
#error You are compiling the benchmarks using a debug build!
#endif

#if SPDLOG_ACTIVE_LEVEL != 6
#error Logging must be disabled globally!
#endif

#include "io/output/VTKWriter.h"
#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include "simulations/LennardJonesLC.h"
#include "simulations/Simulation.h"
#include "utils/Arguments.h"
#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

class LJLCFixture : public ::benchmark::Fixture {
  public:
    Arguments args;
    ParticleContainer pc;

    void SetUp(::benchmark::State &state) {
        // initialize simulation arguments
        args.startTime = 0;
        args.endTime = 1;
        args.delta_t = 0.01;
        args.type = WriterType::NIL;
        args.itFreq = 0x7FFFFFFF;
        args.domainSize = {180, 90};
        args.conditions = {BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE,
                           BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE, BoundaryCondition::REFLECTIVE};
        args.cutoffRadius = 3.0;

        // initialize LJ simulation
        size_t len = state.range(0);
        size_t numParticles = len * len;
        pc.reserve(numParticles);
        Cuboid c{pc, {0., 0., 0.}, {len, len, 1}, {0., 0., 0.}, 1.1225, 1};
        c.initializeParticles();
    }

    void TearDown(::benchmark::State &state) {
        state.counters["Steps"] = args.endTime / args.delta_t;
        state.counters["NumParticles"] = state.range(0) * state.range(0);
    }
};

BENCHMARK_DEFINE_F(LJLCFixture, LJLCNaive)(benchmark::State &st) {
    constexpr int type = 1;
    LennardJonesLC ljlc{pc, args, type};

    /* for some reason, simply calling ljlc.runSimulation() results in a buffer overflow */
    /* i might look into this in the future, but for now, running the simulation manually works */
    auto [cvx, cf] = StrategyFactory::getSimulationFunctions(SimulationType::LJLC, type);
    auto calculateX = cvx.xf;
    auto calculateV = cvx.vf;
    auto calculateF = cf;
    for (auto _ : st) {
        double currentTime = args.startTime;
        while (currentTime < args.endTime) {
            calculateX(ljlc.getParticles(), args.delta_t, &ljlc.getCellContainer());
            calculateF(ljlc.getParticles(), args.epsilon, args.sigma, args.cutoffRadius, &ljlc.getCellContainer());
            calculateV(ljlc.getParticles(), args.delta_t);
            currentTime += args.delta_t;
        }
    }
}

BENCHMARK_DEFINE_F(LJLCFixture, LJLCThirdLaw)(benchmark::State &st) {
    constexpr int type = 0;
    LennardJonesLC ljlc{pc, args, type};
    auto [cvx, cf] = StrategyFactory::getSimulationFunctions(SimulationType::LJLC, type);
    auto calculateX = cvx.xf;
    auto calculateV = cvx.vf;
    auto calculateF = cf;
    for (auto _ : st) {
        double currentTime = args.startTime;
        while (currentTime < args.endTime) {
            calculateX(ljlc.getParticles(), args.delta_t, &ljlc.getCellContainer());
            calculateF(ljlc.getParticles(), args.epsilon, args.sigma, args.cutoffRadius, &ljlc.getCellContainer());
            calculateV(ljlc.getParticles(), args.delta_t);
            currentTime += args.delta_t;
        }
    }
}

BENCHMARK_REGISTER_F(LJLCFixture, LJLCNaive)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(LJLCFixture, LJLCThirdLaw)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);