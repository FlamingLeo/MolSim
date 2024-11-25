# latest version as of 01.11.2024: 1.15.2
set(GTEST_VERSION 1.15.2)

# if gtest is not installed, fetch via git repo
message(CHECK_START "Searching for GTest")
find_package(GTest ${GTEST_VERSION} QUIET)

if (NOT GTEST_FOUND)
    message(CHECK_FAIL "not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG v${GTEST_VERSION}
    )
    FetchContent_MakeAvailable(googletest)
else()
    message(CHECK_PASS "found")
endif()