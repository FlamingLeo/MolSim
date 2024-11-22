#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}"
cd ../input
xsd cxx-tree --cxx-suffix .cpp --hxx-suffix .h --generate-serialization --type-naming ucc --function-naming lcc --output-dir ../src/io/xsd SimulationXSD.xsd
