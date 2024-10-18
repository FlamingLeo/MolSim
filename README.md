# PSE Molecular Dynamics - Group C

The codebase for the Molecular Dynamics Bachelor Practical Course @ TUM of Group C, Winter Term 2024/25.

## Group Members

- [Flavius Leonard Schmidt](https://github.com/FlamingLeo)
- [Luca-Dumitru Drîndea](https://github.com/Luky002)
- [Mara Godeanu](https://github.com/MaraGodeanu)

## Getting Started (preliminary)

### Build Instructions

Before attempting to build, ensure that you have the Xerces-C++ library installed via your system's package manager. On Debian-based systems, simply run the following command:

```bash
sudo apt-get install libxerces-c-dev
```

Afterwards, run the build script to build the executable:

```bash
./build.sh
```

Alternatively, you may build the executable manually using the following commands:

```bash
mkdir build && cd build
cmake ..
make
```

### Run Instructions

After building, the executable will be located in the `build` directory. The syntax from the root directory is as follows:

```bash
build/MolSim <filename>
```
