#include "Pendulo.h"

// El constructor ahora encontrará las variables miembro theta_ y omega_
Pendulo::Pendulo(double theta0, double omega0) : theta_(theta0), omega_(omega0) {}

// Este método ahora tiene una declaración correspondiente en el archivo .h
double Pendulo::individual_energy(const Params& p) const {
    double kinetic_energy = 0.5 * p.m * p.l * p.l * omega_ * omega_;
    double potential_energy = p.m * p.g * p.l * (1.0 - std::cos(theta_));
    return kinetic_energy + potential_energy;
}

// Esta función ahora encontrará el tipo 'Params' correctamente
double calculate_accel(double theta_this, double omega_this, double theta_other, double t, const Params& p) {
    double grav_term = -(p.g / p.l) * std::sin(theta_this);
    double coupling_term = -p.kappa * std::pow(theta_this - theta_other, 2.0);
    // Corrección para la dirección de la fuerza de acoplamiento
    if (theta_this < theta_other) {
        coupling_term = p.kappa * std::pow(theta_this - theta_other, 2.0);
    }
    
    double damping_term = -p.gamma * omega_this;
    double forcing_term = p.F0 * std::cos(p.omega * t);

    // Ecuación: theta_ddot = -gravity - coupling - damping + forcing
    return grav_term + coupling_term + damping_term + forcing_term;
}