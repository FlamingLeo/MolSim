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

static void BM_LJNaive(benchmark::State &state) {
    // initialize simulation arguments
    Arguments args;
    args.startTime = 0;
    args.endTime = state.range(0);
    args.delta_t = 0.025;
    args.type = WriterType::NIL;
    args.itFreq = 0x7FFFFFFF;

    // initialize LJ simulation
    ParticleContainer pc(384);
    Cuboid c1{pc, {0., 0., 0.}, {40, 8, 1}, {0., 0., 0.}, 1.1225, 1};
    Cuboid c2{pc, {15., 15., 0.}, {8, 8, 1}, {0., -10., 0.}, 1.1225, 1};
    c1.initializeParticles();
    c2.initializeParticles();
    LennardJones lj{pc, args, 0};

    // run core simulation functionality for some amount of iterations
    for (auto _ : state) {
        lj.runSimulation();
    }

    // log total steps
    state.counters["Steps"] = args.endTime / args.delta_t;
}

BENCHMARK(BM_LJNaive)->RangeMultiplier(2)->Range(1, 1 << 5)->Unit(benchmark::kMillisecond);