#include "Pendulo.h"

// Constructor
Pendulo::Pendulo(double theta0, double omega0)
    : theta(theta0), omega(omega0) {}

void Pendulo::setState(double theta0, double omega0) {
    theta = theta0;
    omega = omega0;
}

// accel: retorna d2theta/dt2 para este péndulo dado theta del otro
double Pendulo::accel(double theta_other, double omega_self, double t, const Params& p) const {
    // término gravitatorio (no lineal)
    double grav;
    if (p.linear) grav = - (p.g / p.l) * theta;      // aproximación small-angle si linear==true
    else         grav = - (p.g / p.l) * std::sin(theta);

    // acoplamiento: según el enunciado usamos término cuadrático:
    // aparece en las EDO como + kappa * (theta - theta_other)^2 en el lado izquierdo,
    // por lo que en la forma theta_ddot = ... lo tomamos con signo negativo.
    double coupling;
    if (p.linear) {
        // acoplamiento lineal para comparar
        coupling = - p.kappa * (theta - theta_other);
    } else {
        // acoplamiento no lineal (cuadrático)
        double diff = (theta - theta_other);
        coupling = - p.kappa * diff * diff;
        // nota: la dirección del término depende de la derivada del potencial que se definió
    }

    // amortiguamiento
    double damping = - p.gamma * omega_self;

    // forzamiento externo (idéntico en ambos péndulos según enunciado)
    double forcing = p.F0 * std::cos(p.omega_f * t);

    return grav + coupling + damping + forcing;
}

double Pendulo::kineticEnergy(const Params& p) const {
    // Ec = 1/2 m (l * theta_dot)^2
    return 0.5 * p.mass * (p.l * omega) * (p.l * omega);
}

double Pendulo::potentialEnergyGravity(const Params& p) const {
    // Ep = m g l (1 - cos(theta))
    return p.mass * p.g * p.l * (1.0 - std::cos(theta));
}
