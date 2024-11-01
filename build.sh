#!/bin/bash
# check if user is on debian-based system
# if so, use apt-get to check and install missing packages
if [ -f "/etc/debian_version" ]; then
    echo "[BUILD] Running on Debian-based system; required missing packages will be checked and installed automatically."

    # check if libxerces-c-dev is installed
    # if not, install automatically
    echo -n "[BUILD] Checking if libxerces-c is installed... "
    if dpkg -l | grep -qw libxerces-c-dev; then
        echo "found."
    else
        echo "not found! Installing using apt-get..."
        sudo apt-get install -y libxerces-c-dev
    fi
else
    echo "[BUILD] Running on non-Debian-based system; required missing packages WON'T be checked!"
fi

# create new, empty build directory
echo -n "[BUILD] Creating empty build directory... "
rm -rf build
mkdir build
cd build
echo "done."

# run cmake
echo "[BUILD] Calling CMake..."
cmake ..

# build project
echo "[BUILD] Building project..."
make

# project has been built without issues, hopefully
echo "[BUILD] Completed successfully!"