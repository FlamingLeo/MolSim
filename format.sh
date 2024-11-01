#!/bin/bash

# recursively find every C++ source / header file (excluding build directory)...
# ...and print name of file currently being formatted
find . -path ./build -prune -o -regex '.*\.\(cpp\|hpp\|cc\|hh\|c\|h\)' -exec sh -c '
    for file; do
        echo "[FORMAT] Formatting $file..."
        clang-format -i "$file"
    done
' sh {} +

echo "[FORMAT] All files formatted."
