# libxerces-c is required for this project
# if it's not installed, cmake will abort configuration
message(CHECK_START "Searching for XercesC")
find_package(XercesC REQUIRED QUIET)
message(CHECK_PASS "found")