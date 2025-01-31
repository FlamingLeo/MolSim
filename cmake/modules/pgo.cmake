option(PGO_GENERATE "Generate PGO Instrumentation" OFF)
option(PGO_USE "Use PGO Instrumentation" OFF)

# ensure that generation and usage are not on simultaneously
if(PGO_GENERATE AND PGO_USE)
    message(FATAL_ERROR "Cannot create and use PGO instrumentation simultaneously! Consider turning one off.")
endif()

# generate or use PSO
if(PGO_GENERATE)
    message(STATUS "PGO Instrumentation Generation ENABLED")
    set(PGO_OPTIONS "-fprofile-dir=${CMAKE_CURRENT_SOURCE_DIR}/profiledata -fprofile-generate=${CMAKE_CURRENT_SOURCE_DIR}/profiledata")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${PGO_OPTIONS}")
elseif(PGO_USE)
    message(STATUS "Profile Guided Optimization ENABLED")
    set(PGO_OPTIONS "-fprofile-use=${CMAKE_CURRENT_SOURCE_DIR}/profiledata -fprofile-correction")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${PGO_OPTIONS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${PGO_OPTIONS}")
endif()