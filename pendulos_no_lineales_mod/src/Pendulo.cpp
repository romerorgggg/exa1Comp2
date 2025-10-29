#include "Pendulo.h"

Pendulo::Pendulo(double theta0, double omega0)
    : theta(theta0), omega(omega0) {}

void Pendulo::setState(double theta0, double omega0) {
    theta = theta0;
    omega = omega0;
}

double Pendulo::accel(double theta_other, const Params& p) const {
    double grav = -(p.g / p.l) * std::sin(theta);
    double coupling = - p.kappa * (theta - theta_other) * (theta - theta_other);
    return grav + coupling;
}

double Pendulo::kineticEnergy(const Params& p) const {
    return 0.5 * p.mass * (p.l * p.l) * (omega * omega);
}

double Pendulo::potentialEnergyGravity(const Params& p) const {
    return p.mass * p.g * p.l * (1.0 - std::cos(theta));
}
