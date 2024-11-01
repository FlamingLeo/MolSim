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