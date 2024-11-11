#ifndef NDEBUG
#error You are compiling the benchmarks using a debug build!
#endif

#if SPDLOG_ACTIVE_LEVEL != 6
#error Logging must be disabled globally!
#endif

#include "simulations/Verlet.h"
#include "utils/Arguments.h"
#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

static void BM_VerletNaive(benchmark::State &state) {
    // initialize simulation arguments
    Arguments args;
    args.startTime = 0;
    args.endTime = state.range(0);
    args.delta_t = 0.025;
    args.type = WriterType::NIL;
    args.itFreq = 0x7FFFFFFF;

    // initialize planet simulation with input data
    ParticleContainer pc(4);
    pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
    pc.addParticle({0, 1, 0}, {-1, 0, 0}, 3e-06);
    pc.addParticle({0, 5.36, 0}, {-0.425, 0, 0}, 9.55e-4);
    pc.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1.0e-14);
    Verlet v{pc, args, 1};

    // run core simulation functionality for some amount of iterations
    for (auto _ : state) {
        v.runSimulation();
    }

    // log total steps
    state.counters["Steps"] = args.endTime / args.delta_t;
}

static void BM_VerletNewton(benchmark::State &state) {
    // initialize simulation arguments
    Arguments args;
    args.startTime = 0;
    args.endTime = state.range(0);
    args.delta_t = 0.025;
    args.type = WriterType::NIL;
    args.itFreq = 0x7FFFFFFF;

    // initialize planet simulation with input data
    ParticleContainer pc(4);
    pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
    pc.addParticle({0, 1, 0}, {-1, 0, 0}, 3e-06);
    pc.addParticle({0, 5.36, 0}, {-0.425, 0, 0}, 9.55e-4);
    pc.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1.0e-14);
    Verlet v{pc, args, 0};

    // run core simulation functionality for some amount of iterations
    for (auto _ : state) {
        v.runSimulation();
    }

    // log total steps
    state.counters["Steps"] = args.endTime / args.delta_t;
}

BENCHMARK(BM_VerletNaive)->RangeMultiplier(2)->Range(1, 1 << 8);
BENCHMARK(BM_VerletNewton)->RangeMultiplier(2)->Range(1, 1 << 8);