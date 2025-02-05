#!/bin/bash

# NOTE:
# this script is only for personal use and assumes that necessary dependencies are already available.
# as such, there are no checks made that the script will run flawlessly on your machine.

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}/.."
./build.sh -Cgcc-13 -Xg++-13 -bRelWithDebInfo -c