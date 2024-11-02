# latest version as of 01.11.2024: 3.3.0
set(XERCES_VERSION 3.3.0)

# if xerces-c is not installed, fetch via git repo
message(CHECK_START "Searching for XercesC")
find_package(XercesC QUIET)

if (NOT XercesC_FOUND)
    message(CHECK_FAIL "not found, fetching from Apache...")
    include(FetchContent)
    FetchContent_Declare(
        xerces-c
        URL "https://archive.apache.org/dist/xerces/c/3/sources/xerces-c-${XERCES_VERSION}.tar.gz"
        OVERRIDE_FIND_PACKAGE
        DOWNLOAD_EXTRACT_TIMESTAMP On
    )
    FetchContent_MakeAvailable(xerces-c)
else()
    message(CHECK_PASS "found")
endif()