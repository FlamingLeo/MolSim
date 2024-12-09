#ifndef NDEBUG
#error You are compiling the benchmarks using a debug build!
#endif

#if SPDLOG_ACTIVE_LEVEL != 6
#error Logging must be disabled globally!
#endif

#include "objects/Cuboid.h"
#include "objects/ParticleContainer.h"
#include "simulations/LennardJones.h"
#include "utils/Arguments.h"
#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

class LJFixture : public ::benchmark::Fixture {
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

        // initialize LJ simulation
        size_t len = state.range(0);
        size_t numParticles = len * len;
        pc.reserve(numParticles);
        Cuboid c{pc, {0., 0., 0.}, {len, len, 1}, {0., 0., 0.}, 1.1225, 1};
        c.initialize();
    }

    void TearDown(::benchmark::State &state) {
        state.counters["Steps"] = args.endTime / args.delta_t;
        state.counters["NumParticles"] = state.range(0) * state.range(0);
    }
};

BENCHMARK_DEFINE_F(LJFixture, LJNaive)(benchmark::State &st) {
    LennardJones lj{pc, args, 1};
    for (auto _ : st) {
        lj.runSimulation();
    }
}

BENCHMARK_DEFINE_F(LJFixture, LJThirdLaw)(benchmark::State &st) {
    LennardJones lj{pc, args, 0};
    for (auto _ : st) {
        lj.runSimulation();
    }
}

BENCHMARK_DEFINE_F(LJFixture, LJNaiveCutoff)(benchmark::State &st) {
    LennardJones lj{pc, args, 3};
    for (auto _ : st) {
        lj.runSimulation();
    }
}
BENCHMARK_DEFINE_F(LJFixture, LJThirdLawCutoff)(benchmark::State &st) {
    LennardJones lj{pc, args, 2};
    for (auto _ : st) {
        lj.runSimulation();
    }
}

BENCHMARK_REGISTER_F(LJFixture, LJNaive)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(LJFixture, LJThirdLaw)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(LJFixture, LJNaiveCutoff)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(LJFixture, LJThirdLawCutoff)->DenseRange(10, 50, 10)->Unit(benchmark::kMillisecond);