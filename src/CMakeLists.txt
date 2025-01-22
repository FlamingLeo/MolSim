# collect all source files (except MolSim.cpp) and create library
# this is done to enable compatibility with gtest, where we need to include the files as a library...
file(GLOB_RECURSE MD_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM MD_SRC "MolSim.cpp")
add_library(mdsrc ${MD_SRC})

target_include_directories(mdsrc
        SYSTEM PUBLIC
            ${PROJECT_SOURCE_DIR}/libs/libxsd
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mdsrc PUBLIC XercesC::XercesC spdlog::spdlog)
target_compile_definitions(mdsrc PUBLIC SPDLOG_ACTIVE_LEVEL=${SPDLOG_ACTIVE_LEVEL})

# check if openmp is available and enabled
find_package(OpenMP)
message(CHECK_START "Checking OpenMP availability")
if(OpenMP_CXX_FOUND)
    message(CHECK_PASS "found")
    option(ENABLE_OPENMP "Enable OpenMP" ON)
    if(ENABLE_OPENMP)
        target_link_libraries(mdsrc PUBLIC OpenMP::OpenMP_CXX)
    else()
        message(STATUS "OpenMP DISABLED")
    endif()
else()
    message(CHECK_FAIL "not found! Parallelization DISABLED")
endif()

# create executable target from source files
add_executable(MolSim MolSim.cpp)
target_link_libraries(MolSim mdsrc)

# activate all compiler warnings (intel unsupported)
target_compile_options(MolSim PRIVATE -Wall -Wextra -Wpedantic -Werror)

# add spdlog to compiler definition
message(STATUS "SPDLOG_ACTIVE_LEVEL set to ${SPDLOG_ACTIVE_LEVEL} for src.")