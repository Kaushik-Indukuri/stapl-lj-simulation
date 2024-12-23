#pragma once
#include <vector>

struct Particle
{
    double x[3]; // position
    double v[3]; // velocity
    double f[3]; // force
    double mass;
};

class System
{
public:
    System(double box_size_x, double box_size_y, double box_size_z);
    void create_random_atoms(int n_atoms, int seed);
    void calculate_forces();
    void integrate_step(double dt);
    void write_trajectory(const std::string &filename);
    void write_thermo(const std::string &filename);

private:
    std::vector<Particle> particles;
    double box[3];
    double cutoff;
    double epsilon;
    double sigma;
    double potential_energy;
    double kinetic_energy;

    void apply_periodic_boundary();
};
