#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Pendulo.h"

// Prototipos
void get_user_input(int& method, double& dt, double& t_max, Params& p, double& th1_0, double& w1_0, double& th2_0, double& w2_0);
double get_coupling_energy(const Pendulo& p1, const Pendulo& p2, const Params& p);

int main() {
    int method;
    double dt, t_max;
    Params p;
    double th1_0, w1_0, th2_0, w2_0;

    get_user_input(method, dt, t_max, p, th1_0, w1_0, th2_0, w2_0);

    Pendulo p1(th1_0, w1_0);
    Pendulo p2(th2_0, w2_0);

    std::ofstream outfile("results/simulation_data.csv");
    outfile << "t,theta1,omega1,theta2,omega2,E1,E2,ET\n";

    std::string methodName = (method == 1) ? "Runge-Kutta 4" : "Euler-Cromer";
    std::cout << "\n🚀 Iniciando simulación con " << methodName << "...\n";

    for (double t = 0; t <= t_max; t += dt) {
        double e1 = p1.individual_energy(p);
        double e2 = p2.individual_energy(p);
        double ec = get_coupling_energy(p1, p2, p);
        outfile << t << "," << p1.get_theta() << "," << p1.get_omega() << ","
                << p2.get_theta() << "," << p2.get_omega() << ","
                << e1 << "," << e2 << "," << e1 + e2 + ec << "\n";

        if (method == 1) { // RK4
            double th1 = p1.get_theta(), w1 = p1.get_omega();
            double th2 = p2.get_theta(), w2 = p2.get_omega();

            double k1_th1 = dt * w1;
            double k1_w1 = dt * calculate_accel(th1, w1, th2, t, p);
            double k1_th2 = dt * w2;
            double k1_w2 = dt * calculate_accel(th2, w2, th1, t, p);

            double k2_th1 = dt * (w1 + 0.5 * k1_w1);
            double k2_w1 = dt * calculate_accel(th1 + 0.5 * k1_th1, w1 + 0.5 * k1_w1, th2 + 0.5 * k1_th2, t + 0.5 * dt, p);
            double k2_th2 = dt * (w2 + 0.5 * k1_w2);
            double k2_w2 = dt * calculate_accel(th2 + 0.5 * k1_th2, w2 + 0.5 * k1_w2, th1 + 0.5 * k1_th1, t + 0.5 * dt, p);

            double k3_th1 = dt * (w1 + 0.5 * k2_w1);
            double k3_w1 = dt * calculate_accel(th1 + 0.5 * k2_th1, w1 + 0.5 * k2_w1, th2 + 0.5 * k2_th2, t + 0.5 * dt, p);
            double k3_th2 = dt * (w2 + 0.5 * k2_w2);
            double k3_w2 = dt * calculate_accel(th2 + 0.5 * k2_th2, w2 + 0.5 * k2_w2, th1 + 0.5 * k2_th1, t + 0.5 * dt, p);

            double k4_th1 = dt * (w1 + k3_w1);
            double k4_w1 = dt * calculate_accel(th1 + k3_th1, w1 + k3_w1, th2 + k3_th2, t + dt, p);
            double k4_th2 = dt * (w2 + k3_w2);
            double k4_w2 = dt * calculate_accel(th2 + k3_th2, w2 + k3_w2, th1 + k3_th1, t + dt, p);
            
            p1.set_theta(th1 + (k1_th1 + 2*k2_th1 + 2*k3_th1 + k4_th1) / 6.0);
            p1.set_omega(w1 + (k1_w1 + 2*k2_w1 + 2*k3_w1 + k4_w1) / 6.0);
            p2.set_theta(th2 + (k1_th2 + 2*k2_th2 + 2*k3_th2 + k4_th2) / 6.0);
            p2.set_omega(w2 + (k1_w2 + 2*k2_w2 + 2*k3_w2 + k4_w2) / 6.0);

        } else { // Euler-Cromer
            double acc1 = calculate_accel(p1.get_theta(), p1.get_omega(), p2.get_theta(), t, p);
            double acc2 = calculate_accel(p2.get_theta(), p2.get_omega(), p1.get_theta(), t, p);

            p1.set_omega(p1.get_omega() + acc1 * dt);
            p1.set_theta(p1.get_theta() + p1.get_omega() * dt);
            p2.set_omega(p2.get_omega() + acc2 * dt);
            p2.set_theta(p2.get_theta() + p2.get_omega() * dt);
        }
    }
    outfile.close();
    std::cout << "✅ Simulación completada. Datos guardados en: results/simulation_data.csv\n";
    return 0;
}

void get_user_input(int& method, double& dt, double& t_max, Params& p, double& th1_0, double& w1_0, double& th2_0, double& w2_0) {
    std::cout << "=== SIMULACIÓN DE PÉNDULOS ACOPLADOS, AMORTIGUADOS Y FORZADOS ===\n\n";
    std::cout << "Seleccione el método de integración:\n  1. Runge-Kutta 4\n  2. Euler-Cromer\nSu elección [1]: ";
    std::string line;
    std::getline(std::cin, line);
    method = line.empty() ? 1 : std::stoi(line);

    std::cout << "\n--- Parámetros de Simulación ---\n";
    std::cout << "Paso de tiempo (dt) [0.01]: ";
    std::getline(std::cin, line); dt = line.empty() ? 0.01 : std::stod(line);
    std::cout << "Tiempo máximo (t_max) [150.0]: ";
    std::getline(std::cin, line); t_max = line.empty() ? 150.0 : std::stod(line);
    
    std::cout << "\n--- Parámetros Físicos ---\n";
    std::cout << "Longitud (l) [" << p.l << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.l = std::stod(line);
    std::cout << "Masa (m) [" << p.m << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.m = std::stod(line);
    std::cout << "Acoplamiento (kappa) [" << p.kappa << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.kappa = std::stod(line);
    std::cout << "Amortiguamiento (gamma) [" << p.gamma << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.gamma = std::stod(line);
    std::cout << "Amplitud forzamiento (F0) [" << p.F0 << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.F0 = std::stod(line);
    std::cout << "Frecuencia forzamiento (omega) [" << p.omega << "]: ";
    std::getline(std::cin, line); if (!line.empty()) p.omega = std::stod(line);
    
    std::cout << "\n--- Condiciones Iniciales (radianes) ---\n";
    std::cout << "Theta_1 inicial [0.1]: ";
    std::getline(std::cin, line); th1_0 = line.empty() ? 0.1 : std::stod(line);
    std::cout << "Omega_1 inicial [0.0]: ";
    std::getline(std::cin, line); w1_0 = line.empty() ? 0.0 : std::stod(line);
    std::cout << "Theta_2 inicial [0.0]: ";
    std::getline(std::cin, line); th2_0 = line.empty() ? 0.0 : std::stod(line);
    std::cout << "Omega_2 inicial [0.0]: ";
    std::getline(std::cin, line); w2_0 = line.empty() ? 0.0 : std::stod(line);
}

double get_coupling_energy(const Pendulo& p1, const Pendulo& p2, const Params& p) {
    return (p.kappa / 3.0) * std::pow(p1.get_theta() - p2.get_theta(), 3.0);
}