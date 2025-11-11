#ifndef PENDULO_H
#define PENDULO_H

#include <cmath>
#include <string>

/**
 * Clase Pendulo: estado y métodos para un péndulo del sistema acoplado forzado.
 *
 * Ecuación implementada (forma conveniente):
 *   theta_ddot = -gamma*theta_dot - (g/l) * sin(theta) - kappa*(theta - theta_other)^2 + F0 * cos(omega_f * t)
 *
 * Observación: la signatura accel recibe theta_other, omega_self, tiempo t y params.
 */
class Pendulo {
public:
    double theta;   // ángulo (rad)
    double omega;   // velocidad angular (rad/s)

    struct Params {
        double g = 9.81;       // gravedad (m/s^2)
        double l = 1.0;        // longitud (m)
        double kappa = 0.1;    // constante de acoplamiento (no lineal)
        double mass = 1.0;     // masa (kg)
        double gamma = 0.0;    // amortiguamiento
        double F0 = 0.0;       // amplitud de forzamiento
        double omega_f = 1.0;  // frecuencia de forzamiento
        bool linear = false;   // si true usa acoplamiento lineal y sin(theta)≈theta
    };

    Pendulo(double theta0 = 0.0, double omega0 = 0.0);
    void setState(double theta0, double omega0);

    // aceleración angular: dω/dt (incluye forzamiento y amortiguamiento)
    double accel(double theta_other, double omega_self, double t, const Params& p) const;

    // Energías
    double kineticEnergy(const Params& p) const;
    double potentialEnergyGravity(const Params& p) const;
};

#endif // PENDULO_H
