# enable testing with gtest
enable_testing()

# collect all source files and create make target
file(GLOB_RECURSE MD_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_executable(tests ${MD_TESTS})
target_link_libraries(tests PRIVATE GTest::gtest_main mdsrc)

# discover tests included in binary
include(GoogleTest)
gtest_discover_tests(tests)