#pragma once
#include <string>
#include <map>

struct SimulationParams
{
    // Box parameters
    double box_size[3];
    int n_atoms;
    int seed;

    // Force field parameters
    double cutoff;
    double epsilon;
    double sigma;

    // Simulation parameters
    double timestep;
    int n_steps;
    int thermo_freq;

    // Initialization type
    bool use_random; // true for random, false for lattice
    double lattice_constant;
};

class InputReader
{
public:
    static SimulationParams read_input(const std::string &filename);

private:
    static void parse_line(const std::string &line, SimulationParams &params);
    static std::map<std::string, std::string> split_command(const std::string &line);
};
