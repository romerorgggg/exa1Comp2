#ifndef PENDULO_H
#define PENDULO_H

#include <cmath>

// Clase simple que representa un péndulo (ángulo + velocidad angular)
class Pendulo {
public:
    double theta;  // posición angular (rad)
    double omega;  // velocidad angular (rad/s)

    struct Params {
        double g = 9.81;   // gravedad (m/s^2)
        double l = 1.0;    // longitud del péndulo (m)
        double kappa = 0.0; // coeficiente de acoplamiento
        double mass = 1.0; // masa (kg)
    };

    Pendulo(double theta0 = 0.0, double omega0 = 0.0);
    void setState(double theta0, double omega0);

    // devuelve la aceleración angular (d2theta/dt2) dada la theta del otro péndulo
    double accel(double theta_other, const Params& p) const;

    // energías para monitoreo y salida
    double kineticEnergy(const Params& p) const;
    double potentialEnergyGravity(const Params& p) const;
};

#endif // PENDULO_H
