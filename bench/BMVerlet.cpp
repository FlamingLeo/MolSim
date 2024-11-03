#ifndef NDEBUG
#warning You are compiling the benchmarks using a debug build! Measurements will be inaccurate!
#endif

#ifdef SPDLOG_ACTIVE_LEVEL
#undef SPDLOG_ACTIVE_LEVEL
#endif

#define SPDLOG_ACTIVE_LEVEL 6

#include "simulations/Verlet.h"
#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

static void BM_VerletNaive(benchmark::State &state) {
    // initialize planet simulation with input data
    Arguments args;
    ParticleContainer pc(4);
    pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
    pc.addParticle({0, 1, 0}, {-1, 0, 0}, 3e-06);
    pc.addParticle({0, 5.36, 0}, {-0.425, 0, 0}, 9.55e-4);
    pc.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1.0e-14);
    Verlet v{pc, args};

    // run core simulation functionality for some amount of iterations
    for (auto _ : state) {
        v.calculateX();
        v.calculateF();
        v.calculateV();
    }
}

static void BM_VerletNewton(benchmark::State &state) {
    // initialize planet simulation with input data
    Arguments args;
    ParticleContainer pc(4);
    pc.addParticle({0, 0, 0}, {0, 0, 0}, 1);
    pc.addParticle({0, 1, 0}, {-1, 0, 0}, 3e-06);
    pc.addParticle({0, 5.36, 0}, {-0.425, 0, 0}, 9.55e-4);
    pc.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1.0e-14);
    Verlet v{pc, args};

    // run core simulation functionality for some amount of iterations
    for (auto _ : state) {
        v.calculateX();
        v.calculateF_thirdLaw();
        v.calculateV();
    }
}

BENCHMARK(BM_VerletNaive);
BENCHMARK(BM_VerletNewton);