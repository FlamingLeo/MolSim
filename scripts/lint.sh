#!/bin/bash

# NOTE:
# this script is only for personal use and assumes that necessary dependencies are already available.
# as such, there are no checks made that the script will run flawlessly on your machine.

# get directory of script, regardless of where the script is called from
# source: https://stackoverflow.com/a/246128
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}/../src"

cpplint --filter=-legal,-build/include_order,-whitespace/ending_newline,-whitespace/comments,-whitespace/indent,-build/c++17,-runtime/references --linelength=120 --recursive --exclude=io/vtk/* .
