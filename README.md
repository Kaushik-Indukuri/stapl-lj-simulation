# Lennard-Jones Molecular Dynamics Simulation

A simple molecular dynamics simulation program implementing the Lennard-Jones potential. This project is inspired by LAMMPS and integrates the STAPL: Standard Template Adaptive Parallel Library to compare benchmarking results.

## Features

- Lennard-Jones potential with cutoff
- Velocity Verlet integration
- Periodic boundary conditions
- NVE ensemble (microcanonical)
- LAMMPS-style input files
- XYZ trajectory output
- Thermodynamic property calculations

## Building the Project

#### Prerequisites

- CMake (>= 2.8.12.2)
- C++17 compatible compiler
- Make or equivalent build system

#### Setting Up STAPL in Illinois Campus Cluster

```bash
module load boost/1.71.0
module load openmpi/4.1.1-gcc-9.2.0
module show boost/1.71.0
# Look at the LD_LIBRARY_PATH environment variable and copy the path excluding the ‘lib’ directory
# It should be something like: /usr/local/boost/1.71.0
export BOOST_ROOT=<root path for boost>
```

#### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/lj_simulation.git
cd lj_simulation

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
gmake platform=LINUX_gcc stl=${HOME}/stapl-developer/tools/libstdc++/9.2.0
```

If it works then you should have a static stapl library file and a static stapl runtime library file (stapl-developer/lib/libstapl.a and stapl-developer/lib/libstapl_rt.a)

## Running Simulations

#### Basic Usage

```bash
./lj_sim path/to/input_file.in
```

Example:

```bash
./lj_sim ../examples/test.in
```

#### Input File Format

The simulation uses LAMMPS-style input files. Here are the supported commands:

- `create_box <type> <lx> <ly> <lz>`: Define simulation box dimensions
- `create_atoms <type> random <N> <seed>`: Create N atoms randomly
- `pair_style lj/cut <cutoff>`: Set LJ potential cutoff
- `pair_coeff <type1> <type2> <epsilon> <sigma>`: Set LJ parameters
- `timestep <dt>`: Set integration timestep
- `thermo <N>`: Output thermodynamic data every N steps
- `run <steps>`: Run for specified number of steps

Example input file (`test.in`):

### Output Files

The simulation produces two output files in the `output` directory:

1. `trajectory.xyz`: Atomic positions in XYZ format

   ```
   N_atoms
   Comment line (timestep)
   1 x1 y1 z1
   1 x2 y2 z2
   ...
   ```

2. `thermo.dat`: Thermodynamic properties
   ```
   # Step Temperature PotentialEnergy KineticEnergy TotalEnergy
   0 1.44 -0.123 2.160 2.037
   100 1.43 -0.125 2.145 2.020
   ...
   ```

## Units

The simulation uses Lennard-Jones reduced units:

- Length: σ (LJ diameter)
- Energy: ε (LJ well depth)
- Mass: m (particle mass)
- Time: τ = σ√(m/ε)
- Temperature: ε/kB

## Visualization

The trajectory can be visualized using tools like:

- VMD (Visual Molecular Dynamics)
- OVITO
- PyMOL

Example using OVITO:

```bash
ovito output/trajectory.xyz
```

## Authors

- Kaushik Varma Indukuri

## Acknowledgments

- Inspired by LAMMPS (https://www.lammps.org/)
- Based on classical MD algorithms from Allen and Tildesley's "Computer Simulation of Liquids"
