#!/bin/bash

# NOTE:
# this script is only for personal use and assumes that necessary dependencies are already available.
# as such, there are no checks made that the script will run flawlessly on your machine.

# get directory of script, regardless of where the script is called from
# source: https://stackoverflow.com/a/246128
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}/.."

# recursively find every C++ source / header file (excluding build directory)...
# ...and print name of file currently being formatted
find . -path ./build -prune -o -regex '.*\.\(cpp\|hpp\|cc\|hh\|c\|h\)' -exec sh -c '
    for file; do
        echo "[FORMAT] Formatting $file..."
        clang-format -i "$file"
    done
' sh {} +

echo "[FORMAT] All files formatted."
