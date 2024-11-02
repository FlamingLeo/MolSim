# PSE Molecular Dynamics - Group C

The codebase for the Molecular Dynamics Bachelor Practical Course @ TUM of Group C, Winter Term 2024/25.

## Group Members

-   [Flavius Leonard Schmidt](https://github.com/FlamingLeo)
-   [Luca-Dumitru Drîndea](https://github.com/Luky002)
-   [Mara Godeanu](https://github.com/MaraGodeanu)

## Getting Started

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

This project uses the following dependencies:

-   [Xerces-C++](https://xerces.apache.org/xerces-c/) 3.3.0
-   [Googletest](https://github.com/google/googletest) 1.15.2
-   [spdlog](https://github.com/gabime/spdlog) 1.14.1

Aside from [Xerces-C++](https://xerces.apache.org/xerces-c/), which must be installed manually before compiling, the remaining dependencies will be automatically fetched via CMake if they are not present.

### Build Instructions

Before attempting to build, ensure that you have the [Xerces-C++](https://xerces.apache.org/xerces-c/) library installed via your system's package manager. On Debian-based systems, use `apt-get` to install it:

```bash
sudo apt-get install libxerces-c-dev
```

Afterwards, run the build script to build the executable:

```bash
chmod +x build.sh # required once if you get a 'Permission denied' error
./build.sh
```

Alternatively, you may build the executable manually (and optionally disable Doxygen support):

```bash
mkdir build
cd build
cmake .. # no doxygen: 'cmake .. -DENABLE_DOXYGEN=OFF'
make
```

### Run Instructions

After building, the executable will be located in the `build/src` directory. The syntax from inside the directory is:

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

The generated output for use with programs such as [ParaView](https://www.paraview.org/) will be located in the respective subdirectory of the `build` folder (e.g. `build/vtk` for VTK output).

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
