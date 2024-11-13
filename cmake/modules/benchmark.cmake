# usage (in build directory): 
# > cmake -DENABLE_BENCHMARKING=ON ..
# > make bench

# by default, disable benchmarking
option(ENABLE_BENCHMARKING "Enable Benchmarking" OFF)

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

    # define benchmarking macro
    add_compile_definitions(DO_BENCHMARKING)
else()
    message(STATUS "Benchmarking DISABLED")
endif()