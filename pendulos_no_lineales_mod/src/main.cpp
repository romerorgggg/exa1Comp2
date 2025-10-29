// authors romero y garzon
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem>
#include <limits>
#include "Pendulo.h"

using State = std::vector<double>;
Pendulo::Params params;

State derivatives(const State& s) {
    State ds(4);
    double theta1 = s[0];
    double omega1 = s[1];
    double theta2 = s[2];
    double omega2 = s[3];
    ds[0] = omega1;
    ds[2] = omega2;
    Pendulo p1(theta1, omega1);
    Pendulo p2(theta2, omega2);
    ds[1] = p1.accel(theta2, params);
    ds[3] = p2.accel(theta1, params);
    return ds;
}

State rk4_step(const State& s, double dt) {
    State k1 = derivatives(s);
    State s2(4);
    for (int i = 0; i < 4; ++i) s2[i] = s[i] + 0.5 * dt * k1[i];
    State k2 = derivatives(s2);
    State s3(4);
    for (int i = 0; i < 4; ++i) s3[i] = s[i] + 0.5 * dt * k2[i];
    State k3 = derivatives(s3);
    State s4(4);
    for (int i = 0; i < 4; ++i) s4[i] = s[i] + dt * k3[i];
    State k4 = derivatives(s4);
    State snew(4);
    for (int i = 0; i < 4; ++i)
        snew[i] = s[i] + (dt/6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
    return snew;
}

double couplingPotential(double theta1, double theta2, double kappa) {
    return (kappa / 3.0) * std::pow((theta1 - theta2), 3);
}

double totalEnergy(const Pendulo& p1, const Pendulo& p2, const Pendulo::Params& par) {
    double T = p1.kineticEnergy(par) + p2.kineticEnergy(par);
    double Vg = p1.potentialEnergyGravity(par) + p2.potentialEnergyGravity(par);
    double Vc = couplingPotential(p1.theta, p2.theta, par.kappa);
    return T + Vg + Vc;
}

double getUserInput(const std::string& prompt) {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "  Entrada inválida. Por favor, ingrese un número: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

int main() {
    params.g = 9.81; 
    params.l = 1.0; 
    params.kappa = 0.5; 
    params.mass = 1.0;

    double t0 = 0.0, tf = 50.0;
    std::cout << "--- Configuración de la Simulación ---\n";
    std::cout << "Sistema: Péndulos acoplados con interacción no lineal cuadrática\n";
    std::cout << "Parámetros Fijos: g=" << params.g << ", l=" << params.l 
              << ", k=" << params.kappa << ", m=" << params.mass 
              << ", tf=" << tf << "\n";

    double dt = getUserInput("Ingrese el paso de tiempo (dt) (sugerido: 0.001): ");

    std::cout << "\n--- Condiciones Iniciales ---\n";
    double theta1_0 = getUserInput("Ángulo inicial Péndulo 1 (rad) (sugerido: 0.5): ");
    double omega1_0 = getUserInput("Velocidad inicial Péndulo 1 (rad/s) (sugerido: 0.0): ");
    double theta2_0 = getUserInput("Ángulo inicial Péndulo 2 (rad) (sugerido: -0.3): ");
    double omega2_0 = getUserInput("Velocidad inicial Péndulo 2 (rad/s) (sugerido: 0.0): ");

    int steps = static_cast<int>((tf - t0) / dt);

    std::filesystem::create_directories("results");
    std::ofstream fout("results/results.csv");
    fout << std::setprecision(10);
    fout << "# t, theta1, omega1, theta2, omega2, E_total\n";

    State s = {theta1_0, omega1_0, theta2_0, omega2_0};
    Pendulo p1(theta1_0, omega1_0), p2(theta2_0, omega2_0);
    double E0 = totalEnergy(p1, p2, params);
    fout << t0 << ", " << s[0] << ", " << s[1] << ", " << s[2] << ", " << s[3] << ", " << E0 << "\n";

    double t = t0;
    int printEvery = std::max(1, steps / 1000); 
    for (int n = 1; n <= steps; ++n) {
        s = rk4_step(s, dt);
        t += dt;
        if (n % printEvery == 0 || n == steps) {
            p1.setState(s[0], s[1]);
            p2.setState(s[2], s[3]);
            double E = totalEnergy(p1, p2, params);
            fout << t << ", " << s[0] << ", " << s[1] << ", " << s[2] << ", " << s[3] << ", " << E << "\n";
        }
    }

    fout.close();
    std::cout << "Simulación completada. Archivo: results/results.csv\n";
    std::cout << "Para ver las gráficas, ejecute: python3 scripts/plot.py results/results.csv\n";
}
