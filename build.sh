#!/bin/bash

# helper functions
usage() {
  echo "USAGE: $0 [options]" 1>&2
  echo "For more information, type '$0 -h'." 1>&2
  exit 1
}

help() {
  printf "An automatic build script for compiling the program on Unix-based systems.
\033[1mUSAGE\033[0m: $0 [options]

\033[1mOPTIONS\033[0m:
-b <arg> : Sets the type of the compiled binary (default: Release).
  - Debug          : No optimizations, debug information.
  - Release        : High optimization levels, no debug information.
  - RelWithDebInfo : High optimization levels, debug information.
  - MinSizeRel     : Small file size, no debug information.
-c       : Enables benchmarking (default: benchmarking disabled). You MUST compile a Release build.
-d       : Disables Doxygen Makefile target. Incompatible with -m (default: Doxygen enabled).
-f       : Enables fast math optimizations (default: disabled).
-g       : Compiles the program with the '-pg' flag for use with gprof. 
-h       : Prints out a help message. Doesn't build the program.
-j <num> : Sets the number of parallel Makefile jobs to run simultaneously (default: num. of CPU cores).
-l       : Disables automatically installing missing libraries (default: installs automatically)
-m       : Automatically generates documentation after successful compilation. Incompatible with -d (default: off).
-o       : Disables OpenMP functionality.
-O       : Ensures that no outflow simulations will be performed (default: outflow enabled)
           This skips checking particle activity, since all particles should remain active. Be careful when using this option!
-p       : Enables PGO instrumentation code generation (default: off).
-P       : Enables profile-guided compiler optimizations (default: off).
-s <num> : Sets the spdlog level (0: Trace, 1: Debug, 2: Info, 3: Warn, 4: Error, 5: Critical, 6: Off).
           If this option is not explicitly set, the level is based on the build type (Debug: 0, Release: 2).
-t       : Automatically runs tests after successful compilation (default: off).
-C <name>: Sets the C compiler used (default: system-dependent).
-X <name>: Sets the C++ compiler used (default: system-dependent).

\033[1mNOTES\033[0m:
On Debian-based systems, the script will automatically attempt to install missing libraries (unless -l is set) to speed up the compilation process.
This is done using 'sudo apt-get install'. As such, you may be required to enter your password.
"
  exit 0
}

# parse command line options
OPTSTRING=":b:cdfghj:lmoOpPs:tC:X:"
can_check_for_pkgs=true
build_string=""
doxygen_opt=""
fastmath_opt=""
openmp_opt=""
outflow_opt=""
profiling_opt=""
pgo_gen_opt=""
pgo_use_opt=""
benchmarking_opt=""
spdlog_level=""
c_compiler=""
cpp_compiler=""
install_opt=true
run_tests=false
make_documentation=false
num_jobs=$(nproc)
has_updated_apt=false

while getopts ${OPTSTRING} opt; do
  case ${opt} in
  b)
    # choose build type
    if [[ "${OPTARG}" != "Debug" && "${OPTARG}" != "Release" && "${OPTARG}" != "RelWithDebInfo" && "${OPTARG}" != "MinSizeRel" ]]; then
      echo "[ERROR] Invalid build option: ${OPTARG}"
      usage
    fi
    # disable benchmarking with debug builds
    if [[ "${OPTARG}" != "Release" && "${OPTARG}" != "RelWithDebInfo" && "${benchmarking_opt}" == "-DENABLE_BENCHMARKING=ON" ]]; then
      echo "[ERROR] Cannot benchmark with non-release builds!"
      usage
    fi
    echo "[BUILD] Using build option: ${OPTARG}."
    build_string="-DCMAKE_BUILD_TYPE=${OPTARG}"
    ;;
  c)
    # disable benchmarking with debug builds
    if [[ "${build_string}" != "" && "${build_string}" != "-DCMAKE_BUILD_TYPE=Release" && "${build_string}" != "-DCMAKE_BUILD_TYPE=RelWithDebInfo" ]]; then
      echo "[ERROR] Cannot benchmark with non-release builds!"
      usage
    fi

    # disable simultaneous benchmarking and profiling
    if [[ "${profiling_opt}" == "-DENABLE_PROFILING=ON" ]]; then
      echo "[ERROR] Cannot benchmark and profile simultaneously!"
      usage
    fi

    echo "[BUILD] Benchmarking will be enabled."
    benchmarking_opt="-DENABLE_BENCHMARKING=ON"
    ;;
  d)
    # disable doxygen
    if [[ "${make_documentation}" == "true" ]]; then
      echo "[ERROR] Invalid combination - cannot disable Doxygen and simultaneously generate documentation!"
      usage
    fi

    echo "[BUILD] Documentation generation will be disabled."
    doxygen_opt="-DENABLE_DOXYGEN=OFF"
    ;;
  f)
    # enable fast math
    echo "[BUILD] Fast math optimizations will be enabled."
    fastmath_opt="-DENABLE_FAST_MATH=ON"
    ;;
  j)
    # number of makefile jobs
    if [[ "$OPTARG" =~ ^[1-9][0-9]*$ ]]; then
      num_jobs=${OPTARG}
      echo "[BUILD] Set number of Makefile jobs: ${OPTARG}"
    else
      echo "[ERROR] Number of Makefile jobs must be a positive integer."
      usage
    fi
    ;;
  l)
    # exit if library is missing
    echo "[BUILD] Missing libraries will NOT be automatically installed."
    install_opt=false
    ;;
  m)
    # make documentation
    if [[ -n "${doxygen_opt}" ]]; then
      echo "[ERROR] Invalid combination - cannot disable Doxygen and simultaneously generate documentation!"
      usage
    fi

    echo "[BUILD] Documentation will be built after compilation."
    make_documentation=true
    ;;
  o)
    # disable openmp
    echo "[BUILD] OpenMP will be disabled."
    openmp_opt="-DENABLE_OPENMP=OFF"
    ;;
  O)
    # disable outflow simulations
    echo "[BUILD] Outflow simulations will be disabled."
    outflow_opt="-DNO_OUTFLOW=ON"
    ;;
  g)
    # disable profiling with debug builds
    if [[ "${build_string}" != "" && ("${build_string}" != "-DCMAKE_BUILD_TYPE=Release" || "${build_string}" != "-DCMAKE_BUILD_TYPE=RelWithDebInfo") ]]; then
      echo "[ERROR] Cannot profile with non-release builds!"
      usage
    fi

    # disable simultaneous benchmarking and profiling
    if [[ "${benchmarking_opt}" == "-DENABLE_BENCHMARKING=ON" ]]; then
      echo "[ERROR] Cannot benchmark and profile simultaneously!"
      usage
    fi

    # if spdlog isn't set, disable it automatically
    if [[ "${spdlog_level}" == "" ]]; then
      echo "[BUILD] Logging automatically disabled when profiling."
      spdlog_level="-DSPDLOG_LEVEL=6"
    elif [[ "${spdlog_level}" != "-DSPDLOG_LEVEL=6" ]]; then
      echo "[ERROR] Logging must be disabled when profiling!"
      usage
    fi

    # enable profiling
    echo "[BUILD] Profiling will be enabled."
    profiling_opt="-DENABLE_PROFILING=ON"
    ;;
  p)
    # enable pgo instrumentation
    echo "[BUILD] PGO instrumentation generation will be enabled."
    pgo_gen_opt="-DPGO_GENERATE=ON"
    ;;
  P)
    # enable pgo
    echo "[BUILD] Profile guided optimization will be enabled."
    pgo_use_opt="-DPGO_USE=ON"
    ;;
  t)
    # run tests
    echo "[BUILD] Tests will automatically be executed after compilation."
    run_tests=true
    ;;
  s)
    # spdlog level
    if [[ "${OPTARG}" =~ ^[0-6]$ ]]; then
      if [[ "${profiling_opt}" == "-DENABLE_PROFILING=ON" && ${OPTARG} != "6" ]]; then
        echo "[ERROR] Logging must be disabled when profiling!"
        usage
      fi

      spdlog_level="-DSPDLOG_LEVEL=${OPTARG}"
      echo "[BUILD] Set spdlog level: ${OPTARG}"
    else
      echo "[ERROR] Invalid spdlog level; must be between 0 and 6."
      usage
    fi
    ;;
  h)
    # print help message and quit
    help
    ;;
  C)
    # C compiler, no checks
    c_compiler="-DCMAKE_C_COMPILER=${OPTARG}"
    echo "[BUILD] Set C compiler to: ${OPTARG}"
    ;;
  X)
    # C++ compiler, no checks
    cpp_compiler="-DCMAKE_CXX_COMPILER=${OPTARG}"
    echo "[BUILD] Set C compiler to: ${OPTARG}"
    ;;
  ?)
    # unknown option
    echo "[ERROR] Invalid option: -${OPTARG}."
    usage
    ;;
  esac
done

# check if user is on debian-based system
# if so, use apt-get to check and install missing packages
if [ -f "/etc/debian_version" ]; then
  if [[ "${install_opt}" = true ]]; then
    echo "[BUILD] Running on Debian-based system; required missing packages will be installed automatically."
  fi
else
  if [[ "${install_opt}" = true ]]; then
    echo "[BUILD] Running on non-Debian-based system; required missing packages CANNOT be installed automatically!"
    install_opt=0
  fi
fi

# prereq: check if pkg-config is available to find libraries
if command -v pkg-config &>/dev/null; then
  echo "[BUILD] pkg-config is available."
else
  echo -n "[BUILD] pkg-config unavailable... "

  # if the user is on a debian-based system and has chosen to install missing packages, install via apt-get
  if [[ "${install_opt}" = true ]]; then
    echo "installing."
    if [[ "${has_updated_apt}" == "false" ]]; then
      sudo apt-get update
      has_updated_apt=true
    fi
    sudo apt-get install -y pkg-config || {
      echo "[BUILD] Failed to install pkg-config! Will NOT check for missing dependencies!"
      can_check_for_pkgs=false
    }
  else
    echo "automatic package installation disabled. Will NOT check for missing dependencies!"
    can_check_for_pkgs=false
  fi
fi

# check, if pkg-config is installed, if dependencies are installed
# if not, install automatically for quicker compilation (unless -l set)
if [[ "${can_check_for_pkgs}" = true ]]; then
  echo -n "[BUILD] Checking if Xerces-C is installed... "
  if pkg-config --list-all | grep -qw xerces; then
    echo "found."
  else
    echo "not found!"
    # special case: if on linux cluster, attempt to load it using module system
    # otherwise, let user decide locally on debian-based system
    if command -v squeue &>/dev/null; then
      echo "[BUILD] Xerces-C: Running on a Linux Cluster. Attempting to load module..."
      if module load xerces-c &>/dev/null; then
        echo "[BUILD] Successfully loaded the xerces-c module."
      else
        echo "[BUILD] Failed to load the xerces-c module! Aborting."
        exit 1
      fi
    else
      echo "[BUILD] Xerces-C: Running on a local system."
      if [[ "${install_opt}" = true ]]; then
        echo "[BUILD] Installing Xerces-C."
        if [[ "${has_updated_apt}" == "false" ]]; then
          sudo apt-get update
          has_updated_apt=true
        fi
        sudo apt-get install -y libxerces-c-dev || {
          echo "[BUILD] Failed to get libxerces-c-dev, aborting!"
          exit 1
        }
      else
        echo "[BUILD] Automatic Xerces-C installation turned off! Please install Xerces-C manually and try again."
        exit 1
      fi
    fi
  fi

  echo -n "[BUILD] Checking if gtest is installed... "
  if pkg-config --list-all | grep -qw gtest; then
    echo "found."
  else
    if [[ "${install_opt}" = true ]]; then
      echo "not found! Installing using apt-get..."
      if [[ "${has_updated_apt}" == "false" ]]; then
        sudo apt-get update
        has_updated_apt=true
      fi
      sudo apt-get install -y libgtest-dev || echo "[BUILD] Failed to get gtest, will be fetched during compilation."
    else
      echo "not found! Will be fetched during compilation..."
    fi
  fi

  echo -n "[BUILD] Checking if spdlog is installed... "
  if pkg-config --list-all | grep -qw spdlog; then
    echo "found."
  else
    if [[ "${install_opt}" = true ]]; then
      echo "not found! Installing using apt-get..."
      if [[ "${has_updated_apt}" == "false" ]]; then
        sudo apt-get update
        has_updated_apt=true
      fi
      sudo apt-get install -y libspdlog-dev || echo "[BUILD] Failed to get spdlog, will be fetched during compilation."
    else
      echo "not found! Will be fetched during compilation..."
    fi
  fi
fi

# create new, empty build directory
echo -n "[BUILD] Creating empty build directory... "
rm -rf build
mkdir build
cd build
echo "done."

# run cmake
echo "[BUILD] Calling CMake..."
cmake .. ${spdlog_level} ${benchmarking_opt} ${profiling_opt} ${pgo_gen_opt} ${pgo_use_opt} ${doxygen_opt} ${fastmath_opt} ${openmp_opt} ${outflow_opt} ${build_string} ${c_compiler} ${cpp_compiler} || {
  echo "[BUILD] CMake failed! Aborting..."
  exit 1
}

# build project
echo "[BUILD] Building project with ${num_jobs} parallel job(s)..."
make -j ${num_jobs} VERBOSE=1 || {
  echo "[BUILD] Build failed! Aborting..."
  exit 1
}

# if set, create documentation after compilation
if [[ "${make_documentation}" = true ]]; then
  echo "[BUILD] Building documentation..."
  make doc_doxygen || {
    echo "[BUILD] Building documentation failed!"
  }
fi

# if set, run tests after compilation
if [[ "${run_tests}" = true ]]; then
  echo "[BUILD] Running tests..."
  cd tests
  ctest
fi

# post script completed
echo "[BUILD] Script executed successfully!"
