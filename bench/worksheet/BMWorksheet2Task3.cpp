/*
This is the code used to generate the benchmark results for Task 3.
Included for the sake of completeness, commented out because it takes too long.
*/
#if 0
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
        args.endTime = 5;
        args.delta_t = 0.0002;
        args.type = WriterType::NIL;
        args.itFreq = 0x7FFFFFFF;

        // initialize LJ simulation
        pc.reserve(384);
        Cuboid c1{pc, {0., 0., 0.}, {40, 8, 1}, {0., 0., 0.}, 1.1225, 1};
        Cuboid c2{pc, {15., 15., 0.}, {8, 8, 1}, {0., -10., 0.}, 1.1225, 1};
        c1.initializeParticles();
        c2.initializeParticles();
    }

    void TearDown(::benchmark::State &state) { (void)0; }
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

BENCHMARK_REGISTER_F(LJFixture, LJNaive)->Name("Naive")->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(LJFixture, LJThirdLaw)->Name("Third Law")->Unit(benchmark::kMillisecond);
#endif