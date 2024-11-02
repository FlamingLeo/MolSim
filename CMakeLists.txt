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

# include custom cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
include(xerces)
include(doxygen)
include(gtest)
include(spdlog)

# use list files in each subdirectory
add_subdirectory(src)
add_subdirectory(tests)