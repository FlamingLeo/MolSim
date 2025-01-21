# check LTO (link time optimization) support
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

message(CHECK_START "Checking IPO support")
if(supported)
    message(CHECK_PASS "found")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(CHECK_FAIL "not found! LTO DISABLED")
endif()

# macro for checking compiler flags
include(CheckCXXCompilerFlag)
macro(check_and_set_flag flag var)
    check_cxx_compiler_flag(${flag} ${var})
    message(CHECK_START "Checking ${flag}")
    if(${var})
        message(CHECK_PASS "supported")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    else()
        message(CHECK_PASS "unsupported!")
    endif()
endmacro()

# typical optimizations
check_and_set_flag("-march=native" march_supported)
check_and_set_flag("-mtune=native" mtune_supported)
check_and_set_flag("-fdata-sections" fdatasec_supported)
check_and_set_flag("-ffunction-sections" ffuncsec_supported)
check_and_set_flag("-fno-math-errno" fme_supported)
check_and_set_flag("-finline-functions" finline_supported)
check_and_set_flag("-funroll-loops" funroll_supported)
check_and_set_flag("-fomit-frame-pointer" fomit_supported)

# fast math options
option(ENABLE_FAST_MATH "Enable fast math optimizations" ON)
if(ENABLE_FAST_MATH)
    message(STATUS "Fast math optimizations ENABLED")
    check_and_set_flag("-ffast-math" fmath_supported)                   # note: potentially dangerous!
    check_and_set_flag("-funsafe-math-optimizations" funsafe_supported) # note: potentially dangerous!
endif()

# check_and_set_flag("-fprefetch-loop-arrays" fprefetch_supported)