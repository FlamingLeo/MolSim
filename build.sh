#!/bin/bash

# helper functions
usage() {
  echo "USAGE: $0 [-b <Debug | Release | RelWithDebInfo | MinSizeRel>] [-d] [-h] [-l]" 1>&2
  exit 1
}

help() {
  printf "An automatic build script for compiling the program on Unix-based systems.
\033[1mUSAGE\033[0m: $0 [-b <Debug | Release | RelWithDebInfo | MinSizeRel>] [-d] [-h] [-l]

\033[1mOPTIONS\033[0m:
-b : Sets the type of the compiled binary (default: Release).
  - Debug          : No optimizations, debug information.
  - Release        : High optimization levels, no debug information.
  - RelWithDebInfo : High optimization levels, debug information.
  - MinSizeRel     : Small file size, no debug information.
-d : Disables Doxygen Makefile target (default: Doxygen enabled).
-l : Disables automatically installing missing libraries (default: installs automatically)
-h : Prints out a help message. Doesn't build the program.

\033[1mNOTES\033[0m:
On Debian-based systems, the script will automatically attempt to install missing libraries (unless -l is set).
This is done using 'sudo apt-get install'. As such, you may be required to enter your password.
"
  exit 0
}

# parse command line options
OPTSTRING=":b:dlh"
build_string=""
doxygen_opt=""
install_opt=true

while getopts ${OPTSTRING} opt; do
  case ${opt} in
  b)
    # choose build type
    if [[ "${OPTARG}" != "Debug" && "${OPTARG}" != "Release" && "${OPTARG}" != "RelWithDebInfo" && "${OPTARG}" != "MinSizeRel" ]]; then
      echo "ERROR: Invalid build option: ${OPTARG}"
      usage
    fi
    echo "[BUILD] Using build option: ${OPTARG}."
    build_string="-DCMAKE_BUILD_TYPE=${OPTARG}"
    ;;
  d)
    # disable doxygen
    echo "[BUILD] Documentation generation will be disabled."
    doxygen_opt="-DENABLE_DOXYGEN=OFF"
    ;;
  l)
    # exit if library is missing
    echo "[BUILD] Missing libraries will not be automatically installed."
    install_opt=false
    ;;
  h)
    # print help message and quit
    help
    ;;
  ?)
    # unknown option
    echo "ERROR: Invalid option: -${OPTARG}."
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
    echo "[BUILD] Running on non-Debian-based system; required missing packages WON'T be installed automatically!"
    install_opt=0
  fi
fi

# check if libxerces-c-dev is installed
# if not, install automatically (unless -l set)
: '
echo -n "[BUILD] Checking if libxerces-c is installed... "
if pkg-config --list-all | grep -qw xerces; then
  echo "found."
else
  if [[ "${install_opt}" = true ]]; then
    echo "not found! Installing using apt-get..."
    sudo apt-get install -y libxerces-c-dev
  else
    echo "not found! Make sure it is installed and discoverable in your include path, then try again."
    exit 1
  fi
fi
'

# create new, empty build directory
echo -n "[BUILD] Creating empty build directory... "
rm -rf build
mkdir build
cd build
echo "done."

# run cmake
echo "[BUILD] Calling CMake..."
cmake .. ${doxygen_opt} ${build_string}

# build project
echo "[BUILD] Building project..."
make

# project has been built without issues, hopefully
echo "[BUILD] Completed successfully!"
