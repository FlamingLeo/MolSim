# latest version as of 01.11.2024: 1.14.1
set(SPDLOG_VERSION 1.14.1)

# if spdlog is not installed, fetch via git repo
message(CHECK_START "Searching for spdlog")
find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    message(CHECK_FAIL "not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG v${SPDLOG_VERSION}
    )
    FetchContent_MakeAvailable(spdlog)
else()
    message(CHECK_PASS "found")
endif()

# set spdlog level (default: empty)
set(SPDLOG_LEVEL "" CACHE STRING "Specify the SPDLOG_ACTIVE_LEVEL (0 - 6)")
set_property(CACHE SPDLOG_LEVEL PROPERTY STRINGS "0;1;2;3;4;5;6")

# check valid spdlog level or set based on build type
if(SPDLOG_LEVEL)
    # spdlog level must be between 0 and 6
    if(NOT SPDLOG_LEVEL MATCHES "^[0-6]$")
        message(FATAL_ERROR "Invalid SPDLOG_LEVEL: ${SPDLOG_LEVEL}, valid range between 0 and 6.")
    endif()
    set(SPDLOG_ACTIVE_LEVEL ${SPDLOG_LEVEL})
else()
    # set spdlog level based on build type
    # debug   -> 0 (trace)
    # release -> 2 (info)
    message(STATUS "SPDLOG_LEVEL not set, using corresponding build type level")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SPDLOG_ACTIVE_LEVEL 0)
    else()
        set(SPDLOG_ACTIVE_LEVEL 2)
    endif()
endif()

# add spdlog to compiler definition
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=${SPDLOG_ACTIVE_LEVEL})
message(STATUS "SPDLOG_ACTIVE_LEVEL set to ${SPDLOG_ACTIVE_LEVEL}")