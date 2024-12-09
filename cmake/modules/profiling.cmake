# create profiling option (default: off)
option(ENABLE_PROFILING "Compile with -pg for gprof." OFF)

# when profiling, add -pg flag for gprof
if(ENABLE_PROFILING)
    message(STATUS "Profiling ENABLED")
    add_compile_options(-pg)
    add_link_options(-pg)
    
    # define profiling macro
    add_compile_definitions(DO_PROFILING)
else()
    message(STATUS "Profiling DISABLED")
endif()