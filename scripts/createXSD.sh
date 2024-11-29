#!/bin/bash

# NOTE:
# IF EVERYTHING IS LEFT UNCHANGED, THIS WILL CAUSE AN XSD RUNTIME VERSION MISMATCH.
# TO FIX THIS, MANUALLY REMOVE THE CHECK IN SIMULATIONXSD.H, THE CODE WILL STILL WORK FINE.

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}"
cd ../input
xsd cxx-tree --cxx-suffix .cpp --hxx-suffix .h --generate-doxygen --generate-serialization --type-naming ucc --function-naming lcc --output-dir ../src/io/xsd SimulationXSD.xsd
