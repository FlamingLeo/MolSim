# PSE Molecular Dynamics - Group C

The codebase for the Molecular Dynamics Bachelor Practical Course @ TUM of Group C, Winter Term 2024/25.

## Group Members

-   [Flavius Leonard Schmidt](https://github.com/FlamingLeo)
-   [Luca-Dumitru Drîndea](https://github.com/Luky002)
-   [Mara Godeanu](https://github.com/MaraGodeanu)

## Prerequisites

### Tools Required

The code has been developed using the following software on separate machines:

-   [Ubuntu](https://ubuntu.com/) 24.04.1 LTS (minimum: 22.04)
-   [Clang++](https://clang.llvm.org/) 18.1.3 (minimum: 14)
-   [CMake](https://cmake.org/) 3.28.3 (minimum: 3.22)
-   [GNU Make](https://www.gnu.org/software/make/) 4.3 (minimum: 4.3)
-   [Doxygen](https://www.doxygen.nl/) 1.9.8 (optional)
-   [Paraview](https://www.paraview.org/) 5.13.1 (less optional)

Different compilers (e.g. [G++](https://gcc.gnu.org/)) or some older versions (e.g. Clang++ [7](https://en.cppreference.com/w/cpp/17)) may work aswell; however, we provide no guarantee aside from what we used ourselves during development.

### Dependencies 

This project uses the following external C++ libraries:

-   [Xerces-C++](https://xerces.apache.org/xerces-c/) 3.3.0
-   [GoogleTest](https://github.com/google/googletest) 1.15.2
-   [Benchmark](https://github.com/google/benchmark) 1.9.0
-   [spdlog](https://github.com/gabime/spdlog) 1.14.1

If the dependencies are not already installed, they will be automatically fetched via CMake during the build process.

**NOTE**: It is recommended to pre-install the libraries before building to speed up compilation and reduce the size of the `build` folder.

## Getting Started

### Build Instructions (Automatic)

The repository provides a simple, configurable shell script to automate the build process.

```bash
chmod +x build.sh # required once if you get a 'Permission denied' error
./build.sh [options]
```

Following options are supported:

```text
-b : Sets the type of the compiled binary (default: Release).
  - Debug          : No optimizations, debug information.
  - Release        : High optimization levels, no debug information.
  - RelWithDebInfo : High optimization levels, debug information.
  - MinSizeRel     : Small file size, no debug information.
-c : Enables benchmarking (default: benchmarking disabled).
-d : Disables Doxygen Makefile target. Incompatible with -m (default: Doxygen enabled).
-h : Prints out a help message. Doesn't build the program.
-j : Sets the number of parallel Makefile jobs to run simultaneously (default: num. of CPU cores).
-l : Disables automatically installing missing libraries (default: installs automatically)
-m : Automatically generates documentation after successful compilation. Incompatible with -d (default: off).
-s : Sets the spdlog level (0: Trace, 1: Debug, 2: Info, 3: Warn, 4: Error, 5: Critical, 6: Off).
     If this option is not explicitly set, the level is based on the build type (Debug: 0, Release: 2).
-t : Automatically runs tests after successful compilation (default: off).
```

**NOTE**: On Debian-based systems, the script will automatically attempt to install missing libraries (unless `-l` is set) to speed up the compilation process.
This is done using `sudo apt-get install`. As such, you may be required to enter your password.

### Build Instructions (Manual)

Alternatively, you may build the project manually:

```bash
mkdir build
cd build
cmake ..
# -DSPDLOG_LEVEL=<0|1|2|3|4|5|6>
# -DENABLE_DOXYGEN=<OFF|ON>
# -DENABLE_BENCHMARKING=<OFF|ON>
# -DCMAKE_BUILD_TYPE=<Release|Debug|RelWithDebInfo|MinSizeRel>
make
# <MolSim|bench|tests|doc_doxygen|all|clean|help>
```

### Run Instructions

After building, the main executable will be located in the `build/src` directory. The syntax from inside the directory is:

```bash
./MolSim [options] <filename>
```

Currently, the following options are supported:

```text
-s <number>  : Sets the start time (decimal) for a specific simulation (default: 0).
-e <number>  : Sets the end time (decimal) for a specific simulation (default: 1000).
-d <number>  : Sets the time interval between two iterations of a simulation (default: 0.014).
-f <number>  : Sets the output frequency, i.e. after how many iterations a new VTK file should be written (default: 10).
-o <xyz|vtk> : Sets the output file type and directory (default: vtk).
-t <verlet>  : Sets the desired simulation to be performed (default: Verlet) (WIP).
-h           : Prints out a help message. Doesn't perform any simulation.
```

The generated output for use with programs such as [ParaView](https://www.paraview.org/) will be located in the respective subdirectory of the `build/src` folder (e.g. `build/src/vtk` for VTK output).

### Test Instructions

The test executable will be located in the `build/tests` directory. From there, simply run `ctest` to execute the tests.

### Benchmarking Instructions

Benchmarking is **disabled** by default and must be enabled manually using CMake. To perform benchmarking on all simulations, run the built binary inside `build/bench`.

**NOTE**: You may need to [disable CPU scaling](https://github.com/google/benchmark/blob/main/docs/user_guide.md#disabling-cpu-frequency-scaling) when benchmarking for more accurate results. You can do this using `cpupower`.

```bash
sudo cpupower frequency-set --governor performance # disable CPU scaling
# run benchmarks...
sudo cpupower frequency-set --governor powersave   # re-enable CPU scaling
```

## Documentation

The most up-to-date version of the documentation is available [online](https://home.in.tum.de/~scfl/misc/projects/molsim/docs/). Alternatively, you may choose to build the documentation yourself.

### Build Instructions

Documentation is provided via [Doxygen](https://www.doxygen.nl/). As such, in order to build the documentation, you must have Doxygen installed:

```bash
sudo apt-get install doxygen
```

After building the project, you may generate the documentation by running the following command in the `build` directory:

```bash
make doc_doxygen
```

The documentation will be available in the `doxys_documentation` folder inside the root directory.
