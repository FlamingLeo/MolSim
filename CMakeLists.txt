cmake_minimum_required(VERSION 3.20)
project(PSEMolDyn_GroupC)

# set cxx standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type chosen, defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
else()
    message(STATUS "Using build type ${CMAKE_BUILD_TYPE}")
endif(NOT CMAKE_BUILD_TYPE)

# for debug builds, enable address and undefined behavior sanitizers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build chosen, compiling with sanitizers.")
    add_compile_options(-fsanitize=undefined,address)
    add_link_options(-fsanitize=undefined,address)
endif()

# include custom cmake modules to check for libraries
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
include(profiling)
include(cflags)
include(xerces)
include(doxygen)
include(gtest)
include(benchmark)
include(spdlog)
include(pgo)

# add (potentially dangerous) option to ensure that there are no outflow conditions
option(NO_OUTFLOW "Ensure no outflow simulations" OFF)
if(NO_OUTFLOW)
    message(STATUS "Outflow simulations DISABLED! Be careful!")
    add_compile_definitions(NOUTFLOW)
endif()

# use cmake files in each subdirectory
add_subdirectory(src)
add_subdirectory(tests)