#ifndef PENDULO_H
#define PENDULO_H

#include <cmath>

class Pendulo {
public:
    double theta;
    double omega;

    struct Params {
        double g = 9.81;
        double l = 1.0;
        double kappa = 0.0;
        double mass = 1.0;
    };

    Pendulo(double theta0 = 0.0, double omega0 = 0.0);
    void setState(double theta0, double omega0);
    double accel(double theta_other, const Params& p) const;
    double kineticEnergy(const Params& p) const;
    double potentialEnergyGravity(const Params& p) const;
};

#endif // PENDULO_H
