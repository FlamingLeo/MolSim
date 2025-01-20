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
-   [OpenMP](https://www.openmp.org/) 5.0 (minimum: 4.5)
-   [Doxygen](https://www.doxygen.nl/) 1.9.8 (optional)
-   [Paraview](https://www.paraview.org/) 5.13.1 (less optional)

Different compilers (e.g. [G++](https://gcc.gnu.org/) 14) or some older versions (e.g. Clang++ [7](https://en.cppreference.com/w/cpp/17)) may work aswell; however, we provide no guarantee aside from what we used ourselves during development.

### Dependencies

This project uses the following external C++ libraries:

-   [Xerces-C++](https://xerces.apache.org/xerces-c/) 3.3.0¹
-   [GoogleTest](https://github.com/google/googletest) 1.15.2
-   [spdlog](https://github.com/gabime/spdlog) 1.14.1
-   [CodeSynthesis XSD](https://www.codesynthesis.com/products/xsd/) 4.0.0

If the dependencies are not already installed, they will be automatically fetched via CMake during the build process.

**NOTE**: It is recommended to pre-install the libraries before building to speed up compilation and reduce the size of the `build` folder.

¹: Xerces-C++ is not supported when compiling with the Intel C++ compiler.

## Getting Started

### Build Instructions (Automatic)

The repository provides a simple, configurable shell script to automate the build process.

```bash
chmod +x build.sh # required once if you get a 'Permission denied' error
./build.sh [options]
```

Following options are supported:

```text
-b <arg> : Sets the type of the compiled binary (default: Release).
  - Debug          : No optimizations, debug information.
  - Release        : High optimization levels, no debug information.
  - RelWithDebInfo : High optimization levels, debug information.
  - MinSizeRel     : Small file size, no debug information.
-c       : Enables benchmarking (default: benchmarking disabled). You MUST compile a Release build.
-d       : Disables Doxygen Makefile target. Incompatible with -m (default: Doxygen enabled).
-h       : Prints out a help message. Doesn't build the program.
-j <num> : Sets the number of parallel Makefile jobs to run simultaneously (default: num. of CPU cores).
-l       : Disables automatically installing missing libraries (default: installs automatically)
-m       : Automatically generates documentation after successful compilation. Incompatible with -d (default: off).
-o       : Disables OpenMP functionality.
-p       : Compiles the program with the '-pg' flag for use with gprof. 
-s <num> : Sets the spdlog level (0: Trace, 1: Debug, 2: Info, 3: Warn, 4: Error, 5: Critical, 6: Off).
           If this option is not explicitly set, the level is based on the build type (Debug: 0, Release: 2).
-t       : Automatically runs tests after successful compilation (default: off).
-C <name>: Sets the C compiler used (default: system-dependent).
-X <name>: Sets the C++ compiler used (default: system-dependent).
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
# -DENABLE_OPENMP=<OFF|ON>
# -DCMAKE_BUILD_TYPE=<Release|Debug|RelWithDebInfo|MinSizeRel>
make
# <MolSim|bench|tests|doc_doxygen|all|clean|help>
```

### Run Instructions

After building, the main executable will be located in the `build/src` directory. The syntax from inside the directory is:

```text
./MolSim [options] <filename>
```

Currently, the following options are supported:

```text
-s <number>  : Sets the start time (decimal) for a specific simulation (default: simulation-specific).
-e <number>  : Sets the end time (decimal) for a specific simulation (default: simulation-specific).
-d <number>  : Sets the time interval between two iterations of a simulation (default: simulation-specific).
-b <name>    : Sets the base name of the generated files (default: type-specific).
-B <cccccc>  : Sets the conditions to be applied at each boundary (North, South, West, East, Above, Below). c is one of:
  - o        : Outflow (particles get deleted once they leave the domain).
  - r        : Reflective (particles are reflected off the domain boundaries).
-D <x,y,z>   : Sets the domain size (decimal array) for the linked cell method (MUST be specified if not present in input!).
-R <number>  : Sets the cutoff radius (decimal) for the linked cell method (MUST be specified if not present in input!).
-f <number>  : Sets the output frequency, i.e. after how many iterations a new VTK file should be written (default: 10).
-o <type>    : Sets the output file type and directory (default: vtk).
  - vtk      : Generates VTK Unstructured Grid (.vtu) files.
  - xyz      : Generates XYZ (.xyz) files.
  - nil      : Logs to stdout. Used for debugging purposes.
-p <type>    : Sets the parallelization strategy used (default: coarse).
               If OpenMP support is disabled, this option has no effect.
  - coarse   : Uses the standard OpenMP for-loop parallelization strategy.
  - fine     : Uses a finer-grained, task-based parallelization approach.
-t <type>    : Sets the desired simulation to be performed (default: lj).
  - gravity  : Performs a gravitational simulation (t_0 = 0, t_end = 1000, dt = 0.014).
  - lj       : Performs a simulation of Lennard-Jones potential (t_0 = 0, t_end = 5, dt = 0.0002).
-h           : Prints out a help message. Doesn't perform any simulation.
```

The generated output for use with programs such as [ParaView](https://www.paraview.org/) will be located in the respective `vtk` or `xyz` subdirectory from which the program executable was called (i.e. if the program was called from `build/src` with VTK output, the output will be in `build/src/vtk`)

**NOTE**: Logging must be configured at compile time. To change the log level, you must recompile the program [accordingly](#build-instructions-automatic).

### Test Instructions

The test executable will be located in the `build/tests` directory. From there, simply run `ctest` to execute the tests.

### Benchmarking Instructions

Benchmarking is **disabled** by default and must be enabled manually using CMake. When enabled, benchmarking is done automatically when running a simulation in the main executable.

**IMPORTANT**: The log level **must** be at least `INFO`, and you **must** compile a `Release` build. Otherwise, compilation and / or execution will fail.

**NOTE**: You may need to disable CPU scaling when benchmarking for more accurate results. You can do this using `cpupower`.

```bash
sudo cpupower frequency-set --governor performance # disable CPU scaling
# run benchmarks...
sudo cpupower frequency-set --governor powersave   # re-enable CPU scaling
```

## Input Files

**Filename Structure**: `input-lj-w<n>t<m>` = Worksheet `n`, Task `m`.

The program supports XML input files. Currently, the following input files are included in the repository, inside the `input` directory:

-   `input-lj-w2t4.xml`: Simulation of the collision of two particle cuboids.
-   `input-lj-w3t2.xml`: Simulation of the collision of two large particle cuboids, using the linked cell method.
-   `input-lj-w3t2-small.xml`: Simulation of the collision of two small particle cuboids, using the linked cell method.
-   `input-lj-w3t4.xml`: Simulation of a drop of liquid against a reflecting boundary.
-   `input-lj-w4t2-small.xml`: Simulation of the Rayleigh-Taylor instability (small).
-   `input-lj-w4t2-large.xml`: Simulation of the Rayleigh-Taylor instability (large).
-   `input-lj-w4t3-base.xml`: Simulation of the base liquid for the falling drop simulation.
-   `input-lj-w4t3-disc.xml`: Simulation of a falling drop into a liquid.
-   `input-lj-w4t5-small.xml`: Simulation of the Rayleigh-Taylor instability (small), performance contest environment.
-   `input-lj-w4t5-large.xml`: Simulation of the Rayleigh-Taylor instability (large), performance contest environment.
-   `input-lj-w5t1.xml`: Simulation of a membrane.
-   `input-lj-w5t3-coarse.xml`: Simulation of the Rayleigh-Taylor instability in 3D using coarse-grained parallelization.
-   `input-lj-w5t3-fine.xml`: Simulation of the Rayleigh-Taylor instability in 3D using fine-grained parallelization.

**NOTE**: Arguments passed in the command line interface take precedence over arguments included in the XML file. For example, if you have `<startTime>0.0</startTime>` in the input file but specify `-s 5.0` through your terminal, the start time will be 5.0.

### XML Structure

Complete XML input files have the following structure:

```xml
<!-- The complete simulation data is wrapped in a "sim" tag. -->
<sim>
  <!-- (Optional) Simulation arguments are wrapped in "args". This may be omitted, if the simulation can be initialized with default values. -->
  <args>
    <startTime><!-- double --></startTime>                    <!-- start time -->
    <endTime><!-- double --></endTime>                        <!-- end time -->
    <delta_t><!-- double --></delta_t>                        <!-- time step -->
    <frequency><!-- int --></frequency>                       <!-- output frequency -->
    <basename><!-- string --></basename>                      <!-- base name without iteration number of output files -->
    <output><!-- vtk, xyz, nil --></output>                   <!-- output type -->
    <parallelization><!-- coarse, fine --></parallelization>  <!-- parallelization type -->
  </args>
  <!-- A thermostat used to regulate the temperature of the particle system. -->
  <!-- If you do not wish to use the thermostat, set the timeStep value to something larger than the total number of time integration steps and set brownianMotion to false. -->
  <thermostat>
    <!-- The intial temperature of the system. -->
    <init><!-- double --></init>
    <!-- The number of iterations after which the thermostat should be applied. -->
    <timeStep><!-- int --></timeStep>
    <!-- (Optional) The target temperature of the system. -->
    <!-- If the target temperature is not specified, the initial temperature is used. -->
    <target><!-- double --></target>
    <!-- The maximum temperature difference in one thermostat application. -->
    <!-- If it is not specified, the temperature will be updated directly. -->
    <!-- Note that this may cause abrupt changes in the particle velocities. -->
    <deltaT><!-- double --></deltaT>
    <!-- Specify, whether or not particle velocities should be initialized with Brownian Motion in the first iteration. -->
    <brownianMotion><!-- boolean --></brownianMotion>
    <!-- (Optional) Specify, whether or not the thermostat is used for the nano-scale flow simulation. -->
    <!-- By default, this is set to false. -->
    <nanoFlow><!-- boolean --></nanoFlow>
  </thermostat>
  <!-- (Optional) Membrane data used for membrane simulations. -->
  <!-- Currently, this is only compatible with cuboid objects. -->
  <membrane>
    <!-- The stiffness constant. -->
    <stiffness><!-- double --></stiffness>
    <!-- The average bond length of a molecule pair. -->
    <avgBondLength><!-- double --></avgBondLength>
    <!-- The constant upward force along the z-axis. -->
    <zForce><!-- double --></zForce>
  </membrane>
  <!-- The type of the simulation. Must be specified. -->
  <type><!-- gravity, lj --></type>
  <!-- (Optional) Specify whether the simulation should use the linked cells method. -->
  <!-- Currently, the gravitational simulation is unsupported with the linked cells method -->
  <linkedCells><!-- bool --></linkedCells>
  <!-- (Optional) The total number of particles used in the simulation. -->
  <!-- Use this to reserve enough space in the ParticleContainer beforehand to potentially speed up initialization. -->
  <!-- You could theoretically specify any number here, but for optimal memory usage, it should be exact. -->
  <totalParticles><!-- size_t --></totalParticles>
  <!-- (Optional) The dimensions of the simulation. Must be either 2 or 3. -->
  <!-- If this tag isn't specified, the simulation will be 2D by default. -->
  <!-- Note that this may cause unexpected results for 3D input. -->
  <dimensions><!-- unsigned --></dimensions>
  <!-- The simulation molecules. May contain any positive number of "particle", "cuboid" or "disc" entries. -->
  <objects>
    <!-- A simple particle. -->
    <particle>
      <!-- The particle's coordinates. -->
      <position>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </position>
      <!-- The particle's velocity in each direction. -->
      <velocity>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </velocity>
      <!-- The particle's mass (positive). -->
      <mass><!-- double --></mass>
      <!-- The depth of the potential well. Lennard-Jones parameter. -->
      <epsilon><!-- double --></epsilon>
      <!-- The distance where the LJ potential reaches zero. Lennard-Jones parameter. -->
      <sigma><!-- double --></sigma>
      <!-- The particle's type. -->
      <type><!-- int --></type>
    </particle>
    <!-- A cuboid of particles. -->
    <cuboid>
      <!-- The coordinates of the cuboid's lower left origin. -->
      <position>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </position>
      <!-- The velocity of each particle. -->
      <!-- Note that these will be slightly randomized using a Maxwell–Boltzmann distribution. -->
      <velocity>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </velocity>
      <!-- The size of the cuboid in each direction. -->
      <size>
        <x><!-- size_t --></x>
        <y><!-- size_t --></y>
        <z><!-- size_t --></z>
      </size>
      <!-- The distance between the cuboid particles. -->
      <distance><!-- double --></distance>
      <!-- The mass of each cuboid particle. -->
      <mass><!-- double --></mass>
      <!-- The depth of the potential well. Lennard-Jones parameter. -->
      <epsilon><!-- double --></epsilon>
      <!-- The distance where the LJ potential reaches zero. Lennard-Jones parameter. -->
      <sigma><!-- double --></sigma>
      <!-- The particle's type. -->
      <type><!-- int --></type>
    </cuboid>
    <!-- A 2D disc of particles. -->
    <disc>
      <!-- The central origin of the disc. -->
      <position>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </position>
      <!-- The velocity of each particle. -->
      <!-- Note that these will be slightly randomized using a Maxwell–Boltzmann distribution. -->
      <velocity>
        <x><!-- double --></x>
        <y><!-- double --></y>
        <z><!-- double --></z>
      </velocity>
      <!-- The radius of the disc -->
      <radius><!-- int --></radius>
      <!-- The distance between the disc particles. -->
      <distance><!-- double --></distance>
      <!-- The mass of each disc particle. -->
      <mass><!-- double --></mass>
      <!-- The depth of the potential well. Lennard-Jones parameter. -->
      <epsilon><!-- double --></epsilon>
      <!-- The distance where the LJ potential reaches zero. Lennard-Jones parameter. -->
      <sigma><!-- double --></sigma>
      <!-- The particle's type. -->
      <type><!-- int --></type>
    </disc>
  </objects>
</sim>
```

For more information, take a look at the example input files provided in the `input` folder. Alternatively, you can directly look at the XML schema in `input/xsd/SimulationXSD.xsd`.

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

## Miscellaneous

Various miscellaneous scripts are included in the `scripts` directory. **These are merely for development purposes.** There is no guarantee that these scripts will work on any machine.
