# by default, disable benchmarking
option(ENABLE_BENCHMARKING "Enable Benchmarking" OFF)

if(ENABLE_BENCHMARKING)
    # define benchmarking macro
    message(STATUS "Benchmarking ENABLED")
    add_compile_definitions(DO_BENCHMARKING)
else()
    message(STATUS "Benchmarking DISABLED")
endif()