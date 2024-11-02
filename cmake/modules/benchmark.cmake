# by default, enable benchmarking
option(ENABLE_BENCHMARKING "Enable Benchmarking" ON)

if(ENABLE_BENCHMARKING)
    # latest version as of 01.11.2024: 1.9.0
    set(GBENCH_VERSION 1.9.0)
    message(STATUS "Benchmarking ENABLED")
    # if benchmark is not installed, fetch via git repo
    message(CHECK_START "Searching for Benchmark")

    # set this to disable benchmark complaining about gtest...
    set(BENCHMARK_ENABLE_TESTING NO)
    find_package(benchmark QUIET)

    if (NOT benchmark_FOUND)
        message(CHECK_FAIL "not found, fetching from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            benchmark
            GIT_REPOSITORY "https://github.com/google/benchmark.git"
            GIT_TAG v${GBENCH_VERSION}
        )
        FetchContent_MakeAvailable(benchmark)
    else()
        message(CHECK_PASS "found")
    endif()
else()
    message(STATUS "Benchmarking DISABLED")
endif()