# usage (in build directory): 
# > make bench

if(ENABLE_BENCHMARKING)
    # collect all behchmarking files and create make target
    file(GLOB_RECURSE MD_BENCH
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )
    add_executable(bench ${MD_BENCH})
    target_link_libraries(bench PRIVATE benchmark::benchmark benchmark::benchmark_main mdsrc)
    target_compile_definitions(bench PUBLIC SPDLOG_ACTIVE_LEVEL=6)
    message(STATUS "SPDLOG_ACTIVE_LEVEL set to OFF for benchmarks.")
endif()