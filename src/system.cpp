#include "system.h"
#include <stapl/array.hpp>
#include <stapl/algorithm.hpp>
#include <stapl/views/array_view.hpp>
#include <random>
#include <fstream>
#include <cmath>
#include <iostream>

// Define a STAPL pContainer for particles
typedef stapl::array<Particle> particle_array_t;
typedef stapl::array_view<particle_array_t> particle_view_t;

System::System(double box_size_x, double box_size_y, double box_size_z)
{
    box[0] = box_size_x;
    box[1] = box_size_y;
    box[2] = box_size_z;
    cutoff = 2.5;  // LJ cutoff from input
    epsilon = 1.0; // LJ parameter
    sigma = 1.0;   // LJ parameter
    potential_energy = 0.0;
    kinetic_energy = 0.0;
}

void System::create_random_atoms(int n_atoms, int seed)
{
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0.0, 1.0);

    particles.resize(n_atoms);
    for (auto &p : particles)
    {
        // Random positions
        for (int i = 0; i < 3; i++)
        {
            p.x[i] = dis(gen) * box[i];
            p.v[i] = dis(gen) * 2.0 - 1.0; // Random velocities [-1,1]
        }
        p.mass = 1.0;
    }
}

void System::calculate_forces()
{
    // Create a STAPL array for particles
    particle_array_t particles_stapl(particles.size());
    particle_view_t particles_view(particles_stapl);

    // Copy data from standard vector to STAPL array
    stapl::do_once([&]()
                   {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles_stapl[i] = particles[i];
        } });

    // Parallel force calculation using STAPL
    stapl::map_func([&](Particle &p1, Particle &p2)
                    {
        double dx[3];
        double r2 = 0.0;

        // Calculate distance with minimum image convention
        for (int k = 0; k < 3; k++) {
            dx[k] = p1.x[k] - p2.x[k];
            dx[k] -= box[k] * round(dx[k] / box[k]);
            r2 += dx[k] * dx[k];
        }

        if (r2 < cutoff * cutoff) {
            double r6i = 1.0 / (r2 * r2 * r2);
            double r12i = r6i * r6i;
            double force_magnitude = 24.0 * epsilon * (2.0 * r12i - r6i) / r2;

            for (int k = 0; k < 3; k++) {
                double force = force_magnitude * dx[k];
                p1.f[k] += force;
                p2.f[k] -= force;
            }
        } }, particles_view, particles_view);

    // Copy data back to standard vector
    stapl::do_once([&]()
                   {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i] = particles_stapl[i];
        } });
}

void System::integrate_step(double dt)
{
    kinetic_energy = 0.0;

    // Velocity Verlet integration
    for (auto &p : particles)
    {
        // First half of velocity update
        for (int i = 0; i < 3; i++)
        {
            p.v[i] += 0.5 * dt * p.f[i] / p.mass;
            p.x[i] += dt * p.v[i];
        }
    }

    apply_periodic_boundary();
    calculate_forces();

    // Second half of velocity update
    for (auto &p : particles)
    {
        for (int i = 0; i < 3; i++)
        {
            p.v[i] += 0.5 * dt * p.f[i] / p.mass;
        }
        // Calculate kinetic energy
        for (int i = 0; i < 3; i++)
        {
            kinetic_energy += 0.5 * p.mass * p.v[i] * p.v[i];
        }
    }
}

void System::apply_periodic_boundary()
{
    for (auto &p : particles)
    {
        for (int i = 0; i < 3; i++)
        {
            p.x[i] -= box[i] * floor(p.x[i] / box[i]);
        }
    }
}

void System::write_trajectory(const std::string &filename)
{
    std::ofstream file(filename, std::ios::app | std::ios::out);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open trajectory file: " << filename << std::endl;
        return;
    }

    file << particles.size() << "\n";
    file << "Atoms. Timestep: " << "\n";

    for (const auto &p : particles)
    {
        file << "1 " << p.x[0] << " " << p.x[1] << " " << p.x[2] << "\n";
    }
    file.close();
}

void System::write_thermo(const std::string &filename)
{
    static bool first_write = true;
    std::ofstream file;

    if (first_write)
    {
        file.open(filename, std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not create thermo file: " << filename << std::endl;
            return;
        }
        file << "# Step Temperature PotentialEnergy KineticEnergy TotalEnergy\n";
        first_write = false;
    }
    else
    {
        file.open(filename, std::ios::app | std::ios::out);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open thermo file: " << filename << std::endl;
            return;
        }
    }

    double temperature = kinetic_energy / (1.5 * particles.size());
    double total_energy = potential_energy + kinetic_energy;

    file << "0 " << temperature << " " << potential_energy << " "
         << kinetic_energy << " " << total_energy << "\n";
    file.close();
}
