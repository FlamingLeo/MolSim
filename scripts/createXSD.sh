#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
cd "${SCRIPT_DIR}"
cd ../input/xsd
rm ../../src/io/xsd/SimulationXSD.*
xsd cxx-tree --cxx-suffix .cpp --hxx-suffix .h --generate-doxygen --generate-serialization --type-naming ucc --function-naming lcc --output-dir ../../src/io/xsd SimulationXSD.xsd
sed -i '/#if (LIBXSD_VERSION !=/,+2d' ../../src/io/xsd/SimulationXSD.h
clang-format -i ../../src/io/xsd/SimulationXSD.h
clang-format -i ../../src/io/xsd/SimulationXSD.cpp