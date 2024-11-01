# usage (in build directory): 
# > cmake -DENABLE_DOXYGEN=ON ..
# > make doc_doxygen

# look for doxygen...
message(CHECK_START "Searching for Doxygen")
find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    message(CHECK_PASS "found")

    # create build option for the target
    option(ENABLE_DOXYGEN "Enable Doxygen documentation generation" ON)

    if (ENABLE_DOXYGEN)
        message(STATUS "Doxygen documentation generation ENABLED via 'make doc_doxygen'")
        # find doxyfile in root directory; maybe use CMAKE_SOURCE_DIR?
        set(DOXYFILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

        # sanity check, in case doxyfile somehow gets deleted
        if (EXISTS ${DOXYFILE_PATH})
            # add custom target for building documentation
            add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_PATH}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Generating Doxygen documentation..."
                VERBATIM
            )

            # make doc_doxygen optional by excluding it from "make all"
            set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)
        else()
            message(WARNING "Doxyfile not found at ${DOXYFILE_PATH}, cannot generate documentation.")
        endif()
    else()
        message(STATUS "Doxygen documentation generation DISABLED")
    endif()
else()
    message(CHECK_FAIL "not found! Documentation generation DISABLED")
endif() 