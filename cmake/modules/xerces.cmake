# if xerces-c is not installed, abort
message(CHECK_START "Searching for XercesC")
find_package(XercesC REQUIRED)

if (XercesC_FOUND)
    message(CHECK_PASS "found")
endif()