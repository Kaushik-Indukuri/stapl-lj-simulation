#include "input.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

SimulationParams InputReader::read_input(const std::string &filename)
{
    SimulationParams params = {
        {10.0, 10.0, 10.0}, // default box size
        100,                // default n_atoms
        12345,              // default seed
        2.5,                // default cutoff
        1.0,                // default epsilon
        1.0,                // default sigma
        0.005,              // default timestep
        1000,               // default n_steps
        100,                // default thermo_freq
        true,               // default to random initialization
        0.8442              // default lattice constant
    };

    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Could not open input file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        parse_line(line, params);
    }

    return params;
}

void InputReader::parse_line(const std::string &line, SimulationParams &params)
{
    auto command = split_command(line);

    if (command.empty())
        return;

    const auto &cmd = command.begin()->first;
    const auto &args = command.begin()->second;

    std::istringstream iss(args);

    if (cmd == "create_box")
    {
        int type;
        iss >> type >> params.box_size[0] >> params.box_size[1] >> params.box_size[2];
    }
    else if (cmd == "create_atoms")
    {
        int type;
        std::string method;
        iss >> type >> method;
        if (method == "random")
        {
            iss >> params.n_atoms >> params.seed;
            params.use_random = true;
        }
    }
    else if (cmd == "pair_style")
    {
        std::string style;
        iss >> style >> params.cutoff;
    }
    else if (cmd == "pair_coeff")
    {
        int type1, type2;
        iss >> type1 >> type2 >> params.epsilon >> params.sigma;
    }
    else if (cmd == "timestep")
    {
        iss >> params.timestep;
    }
    else if (cmd == "run")
    {
        iss >> params.n_steps;
    }
    else if (cmd == "thermo")
    {
        iss >> params.thermo_freq;
    }
    else if (cmd == "lattice")
    {
        std::string type;
        iss >> type >> params.lattice_constant;
        params.use_random = false;
    }
}

std::map<std::string, std::string> InputReader::split_command(const std::string &line)
{
    std::map<std::string, std::string> result;
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    std::string remaining;
    std::getline(iss, remaining);

    if (!command.empty())
    {
        result[command] = remaining;
    }

    return result;
}
