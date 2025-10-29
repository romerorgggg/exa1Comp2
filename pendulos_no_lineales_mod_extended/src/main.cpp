// main.cpp
// Versión extendida: permite ingresar varios valores de kappa (dobles) en una sola línea
// y corre una simulación completa por cada kappa, guardando resultados independientes.
//
// Comentarios estilo "humano" para facilitar la lectura del código en la entrega.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <limits>
#include <cmath>
#include "Pendulo.h"

using State = std::vector<double>;
Pendulo::Params params; // parámetros globales compartidos

// Calcula derivadas del sistema de 4 variables: [theta1, omega1, theta2, omega2]
State derivatives(const State& s) {
    State ds(4);
    double theta1 = s[0], omega1 = s[1];
    double theta2 = s[2], omega2 = s[3];

    // dtheta/dt = omega
    ds[0] = omega1;
    ds[2] = omega2;

    // Usamos la clase Pendulo únicamente para aprovechar accel()
    Pendulo p1(theta1, omega1), p2(theta2, omega2);
    ds[1] = p1.accel(theta2, params); // domega1/dt
    ds[3] = p2.accel(theta1, params); // domega2/dt
    return ds;
}

// Paso RK4 para avanzar el vector de estado un dt
State rk4_step(const State& s, double dt) {
    State k1 = derivatives(s);
    State s2(4), s3(4), s4(4), snew(4);

    for (int i = 0; i < 4; ++i) s2[i] = s[i] + 0.5 * dt * k1[i];
    State k2 = derivatives(s2);

    for (int i = 0; i < 4; ++i) s3[i] = s[i] + 0.5 * dt * k2[i];
    State k3 = derivatives(s3);

    for (int i = 0; i < 4; ++i) s4[i] = s[i] + dt * k3[i];
    State k4 = derivatives(s4);

    for (int i = 0; i < 4; ++i)
        snew[i] = s[i] + (dt/6.0) * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]);

    return snew;
}

// Potencial de acoplamiento elegido para monitoreo (integral del término de fuerza kappa*(d)^2)
// si F = - dV/dd, entonces V = kappa/3 * d^3  con d = (theta1 - theta2)
double couplingPotential(double theta1, double theta2, double kappa) {
    return (kappa / 3.0) * std::pow((theta1 - theta2), 3);
}

// Energía total del sistema (T1 + T2 + Vg1 + Vg2 + Vc)
double totalEnergy(const Pendulo& p1, const Pendulo& p2, const Pendulo::Params& par) {
    double T = p1.kineticEnergy(par) + p2.kineticEnergy(par);
    double Vg = p1.potentialEnergyGravity(par) + p2.potentialEnergyGravity(par);
    double Vc = couplingPotential(p1.theta, p2.theta, par.kappa);
    return T + Vg + Vc;
}

// Lee una línea completa y parsea doubles (valores de kappa) separados por espacios
std::vector<double> readKappasFromLine() {
    std::vector<double> kappas;
    std::string line;
    std::getline(std::cin, line); // lee la línea completa (puede estar vacía si se heredó newline)
    if(line.empty()) std::getline(std::cin, line); // si quedó un '\n' anterior, leer otra vez

    std::istringstream iss(line);
    double x;
    while (iss >> x) kappas.push_back(x);
    return kappas;
}

// Función que ejecuta la simulación y escribe un archivo CSV con los resultados.
// Recibe el nombre de archivo y condiciones iniciales; mantiene 'params' como entrada.
void run_simulation_and_save(const std::string& filename,
                             double dt, double tf,
                             double theta1_0, double omega1_0,
                             double theta2_0, double omega2_0) {
    int steps = static_cast<int>((tf) / dt);

    // Aseguramos que carpeta results exista
    std::filesystem::create_directories("results");
    std::ofstream fout(filename);
    fout << std::setprecision(10);
    fout << "# t, theta1, omega1, theta2, omega2, E_total\n";

    State s = {theta1_0, omega1_0, theta2_0, omega2_0};
    Pendulo p1(theta1_0, omega1_0), p2(theta2_0, omega2_0);

    double t = 0.0;
    // Guardamos un número manejable de puntos (no cada dt si steps es grande)
    int printEvery = std::max(1, steps / 1000);

    // Guardar estado inicial
    double E0 = totalEnergy(p1, p2, params);
    fout << t << ", " << s[0] << ", " << s[1] << ", " << s[2] << ", " << s[3] << ", " << E0 << "\n";

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
}

int main() {
    // Parámetros físicos por defecto (se pueden cambiar aquí)
    params.g = 9.81;
    params.l = 1.0;
    params.mass = 1.0;

    // Parámetros de simulación (pedimos dt y condiciones iniciales al usuario)
    double tf = 50.0;
    double dt;

    std::cout << "=== Péndulos acoplados (bucle sobre varios k) ===\n";
    std::cout << "Ingrese los valores de k separados por espacios (ej: 0.2 0.5 1.0): \n";
    std::vector<double> kappas = readKappasFromLine();

    if (kappas.empty()) {
        std::cout << "No se ingresaron valores de k. Saliendo.\n";
        return 0;
    }

    std::cout << "Ingrese el paso de tiempo dt (sugerido 0.001): "; std::cin >> dt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer

    std::cout << "-- Condiciones iniciales --\n";
    double theta1_0, omega1_0, theta2_0, omega2_0;
    std::cout << "Ángulo inicial P1 (rad, sugerido 0.5): "; std::cin >> theta1_0;
    std::cout << "Velocidad inicial P1 (rad/s, sugerido 0.0): "; std::cin >> omega1_0;
    std::cout << "Ángulo inicial P2 (rad, sugerido -0.3): "; std::cin >> theta2_0;
    std::cout << "Velocidad inicial P2 (rad/s, sugerido 0.0): "; std::cin >> omega2_0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer

    // Ejecutar una simulación por cada k ingresado
    for (double k : kappas) {
        params.kappa = k;
       // create a filename friendly string for k
        std::ostringstream ss; ss << std::fixed << std::setprecision(3) << k;
        std::string kstr = ss.str();
        for (char &c : kstr) if (c == '.') c = 'p';
        std::string filename = "results/results_kappa" + kstr + ".csv";

        std::cout << "Ejecutando simulación para k = " << k << "  -> " << filename << "\n";
        run_simulation_and_save(filename, dt, tf, theta1_0, omega1_0, theta2_0, omega2_0);
        std::cout << "Guardado: " << filename << "\n";
    }

    std::cout << "Todas las simulaciones completadas. Use scripts/plot_compare.py para generar comparaciones.\n";
    return 0;
}
