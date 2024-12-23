#include "system.h"
#include "input.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Read input parameters
    SimulationParams params;
    try
    {
        params = InputReader::read_input(argv[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading input file: " << e.what() << std::endl;
        return 1;
    }

    // Output files
    std::string traj_file = "../output/trajectory.xyz";
    std::string thermo_file = "../output/thermo.dat";

    // Try to open files with explicit error checking
    {
        std::ofstream test_traj(traj_file);
        if (!test_traj)
        {
            std::cerr << "Error: Cannot open trajectory file: " << traj_file << std::endl;
            return 1;
        }
        test_traj.close();

        std::ofstream test_thermo(thermo_file);
        if (!test_thermo)
        {
            std::cerr << "Error: Cannot open thermo file: " << thermo_file << std::endl;
            return 1;
        }
        test_thermo.close();
    }

    // Initialize system based on input file parameters
    System system(params.box_size[0], params.box_size[1], params.box_size[2]);

    if (params.use_random)
    {
        system.create_random_atoms(params.n_atoms, params.seed);
    }
    else
    {
        // TODO: Implement lattice initialization
        std::cerr << "Lattice initialization not yet implemented" << std::endl;
        return 1;
    }

    // Main MD loop
    for (int step = 0; step < params.n_steps; step++)
    {
        system.calculate_forces();
        system.integrate_step(params.timestep);

        if (step % params.thermo_freq == 0)
        {
            system.write_trajectory(traj_file);
            system.write_thermo(thermo_file);
        }
    }

    return 0;
}
