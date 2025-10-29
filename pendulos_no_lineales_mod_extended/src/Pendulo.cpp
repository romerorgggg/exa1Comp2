#include "Pendulo.h"

// Constructor sencillo: inicializa theta y omega
Pendulo::Pendulo(double theta0, double omega0)
    : theta(theta0), omega(omega0) {}

void Pendulo::setState(double theta0, double omega0) {
    theta = theta0;
    omega = omega0;
}

// Ecuación de movimiento (aceleración angular):
// d^2 theta / dt^2 = - (g/l) sin(theta) - kappa * (theta - theta_other)^2
// Comentarios: el término de acoplamiento tiene signo negativo tal como en el enunciado.
double Pendulo::accel(double theta_other, const Params& p) const {
    // término restaurador gravitacional
    double grav = -(p.g / p.l) * std::sin(theta);

    // acoplamiento no lineal cuadrático
    double coupling = - p.kappa * (theta - theta_other) * (theta - theta_other);

    return grav + coupling;
}

// Energía cinética rotacional: T = 1/2 * m * (l * omega)^2 = 1/2 m l^2 omega^2
double Pendulo::kineticEnergy(const Params& p) const {
    return 0.5 * p.mass * (p.l * p.l) * (omega * omega);
}

// Energía potencial gravitacional medida desde la posición vertical estable: V = m g l (1 - cos theta)
double Pendulo::potentialEnergyGravity(const Params& p) const {
    return p.mass * p.g * p.l * (1.0 - std::cos(theta));
}
