#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "Pendulo.h"

using std::string;

struct State { double th1, w1, th2, w2; };
Pendulo::Params params;
string integrator = "RK4"; // por defecto RK4

// Derivadas
State derivatives(const State& s, double t) {
    State ds;
    ds.th1 = s.w1;
    ds.th2 = s.w2;
    ds.w1 = Pendulo(s.th1, s.w1).accel(s.th2, s.w1, t, params);
    ds.w2 = Pendulo(s.th2, s.w2).accel(s.th1, s.w2, t, params);
    return ds;
}

// RK4 paso
State rk4_step(const State& s, double t, double dt) {
    State k1 = derivatives(s, t);
    State s2 = { s.th1 + 0.5*dt*k1.th1, s.w1 + 0.5*dt*k1.w1, s.th2 + 0.5*dt*k1.th2, s.w2 + 0.5*dt*k1.w2 };
    State k2 = derivatives(s2, t + 0.5*dt);
    State s3 = { s.th1 + 0.5*dt*k2.th1, s.w1 + 0.5*dt*k2.w1, s.th2 + 0.5*dt*k2.th2, s.w2 + 0.5*dt*k2.w2 };
    State k3 = derivatives(s3, t + 0.5*dt);
    State s4 = { s.th1 + dt*k3.th1, s.w1 + dt*k3.w1, s.th2 + dt*k3.th2, s.w2 + dt*k3.w2 };
    State k4 = derivatives(s4, t + dt);

    State ns;
    ns.th1 = s.th1 + (dt/6.0)*(k1.th1 + 2*k2.th1 + 2*k3.th1 + k4.th1);
    ns.w1  = s.w1  + (dt/6.0)*(k1.w1  + 2*k2.w1  + 2*k3.w1  + k4.w1);
    ns.th2 = s.th2 + (dt/6.0)*(k1.th2 + 2*k2.th2 + 2*k3.th2 + k4.th2);
    ns.w2  = s.w2  + (dt/6.0)*(k1.w2  + 2*k2.w2  + 2*k3.w2  + k4.w2);
    return ns;
}

// Euler-Cromer paso
State euler_cromer_step(const State& s, double t, double dt) {
    State ds = derivatives(s, t);
    State ns = s;
    ns.w1 += ds.w1 * dt; ns.th1 += ns.w1 * dt;
    ns.w2 += ds.w2 * dt; ns.th2 += ns.w2 * dt;
    return ns;
}

// Potencial de acoplamiento (para energía)
double couplingPotential(double th1, double th2, const Pendulo::Params& p) {
    // Vc = kappa/3 * (th1 - th2)^3   (según enunciado)
    double diff = th1 - th2;
    return (p.kappa / 3.0) * std::pow(diff, 3);
}

// Energía total del sistema
double totalEnergy(const Pendulo& p1, const Pendulo& p2, const Pendulo::Params& p) {
    double T = p1.kineticEnergy(p) + p2.kineticEnergy(p);
    double Vg = p1.potentialEnergyGravity(p) + p2.potentialEnergyGravity(p);
    double Vc = couplingPotential(p1.theta, p2.theta, p);
    return T + Vg + Vc;
}

// Escribir CSV con E1, E2, E_total
void writeCSV(const string& path, const std::vector<double>& t,
              const std::vector<State>& states,
              const std::vector<double>& E1,
              const std::vector<double>& E2,
              const std::vector<double>& Et) {
    std::ofstream fout(path);
    fout << std::setprecision(10);
    fout << "t,theta1,omega1,theta2,omega2,E1,E2,E_total\n";
    for (size_t i = 0; i < t.size(); ++i) {
        fout << t[i] << "," << states[i].th1 << "," << states[i].w1 << ","
             << states[i].th2 << "," << states[i].w2 << "," << E1[i] << "," << E2[i] << "," << Et[i] << "\n";
    }
    fout.close();
}

// Simulación general que guarda E1, E2, E_total
string run_simulation(const string &outname, State s0, double t0, double tf, double dt, const string &method) {
    std::filesystem::create_directories("results");
    std::vector<double> times;
    std::vector<State> states;
    std::vector<double> E1, E2, Et;

    double t = t0;
    State s = s0;
    int steps = int((tf - t0) / dt);

    for (int i = 0; i <= steps; ++i) {
        if (i % 25 == 0) {
            Pendulo p1(s.th1, s.w1), p2(s.th2, s.w2);
            times.push_back(t);
            states.push_back(s);
            E1.push_back(p1.kineticEnergy(params) + p1.potentialEnergyGravity(params));
            E2.push_back(p2.kineticEnergy(params) + p2.potentialEnergyGravity(params));
            Et.push_back(totalEnergy(p1,p2,params));
        }
        if (method == "EC") s = euler_cromer_step(s, t, dt);
        else s = rk4_step(s, t, dt);
        t += dt;
    }

    string out = "results/" + outname;
    writeCSV(out, times, states, E1, E2, Et);
    return out;
}

// Barrido de resonancia: escribe omega_f, max_amp_theta1
string run_resonance_sweep(const string &outname, State s0, double t0, double tf, double dt,
                           double omega_min, double omega_max, int nsteps) {
    std::filesystem::create_directories("results");
    std::ofstream fout("results/" + outname);
    fout << "omega_f,max_amp_theta1\n";
    double domega = (omega_max - omega_min) / (nsteps - 1);
    for (int k = 0; k < nsteps; ++k) {
        double omega_f = omega_min + k * domega;
        params.omega_f = omega_f;
        string tmp = run_simulation("tmp_res.csv", s0, t0, tf, dt, integrator);
        // leer tmp_res y calcular max |theta1|
        double maxamp = 0.0;
        std::ifstream fin("results/tmp_res.csv");
        string line; std::getline(fin,line); // cabecera
        while (std::getline(fin,line)) {
            if (line.empty()) continue;
            double tt,th1,w1,th2,w2,e1,e2,et;
            sscanf(line.c_str(), "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                   &tt,&th1,&w1,&th2,&w2,&e1,&e2,&et);
            if (std::fabs(th1) > maxamp) maxamp = std::fabs(th1);
        }
        fin.close();
        fout << omega_f << "," << maxamp << "\n";
    }
    fout.close();
    return "results/" + outname;
}

// Menú interactivo principal
int main() {
    // default params (ajusta si quieres)
    params.g = 9.81; params.l = 1.0; params.kappa = 0.1; params.mass = 1.0;
    params.gamma = 0.05; params.F0 = 0.0; params.omega_f = 1.0;
    params.linear = false;
    integrator = "RK4";

    State s0 = {0.5, 0.0, -0.3, 0.0};

    while (true) {
        std::cout << "\n=== Péndulos acoplados forzados (interactivo) ===\n"
                  << "1) Transferencia de energía (energy)\n"
                  << "2) Barrido de resonancia (resonance)\n"
                  << "3) Régimen caótico (chaos)\n"
                  << "4) Generar gráficas (Python)\n"
                  << "5) Cambiar parámetros (kappa,gamma,F0,omega_f,linear)\n"
                  << "6) Cambiar integrador (actual: " << integrator << ")\n"
                  << "7) Comprimir results (pendulos_results.zip)\n"
                  << "8) Salir\n"
                  << "Elige opción [1-8]: ";
        int opt;
        if (!(std::cin >> opt)) { std::cin.clear(); std::string d; std::getline(std::cin,d); continue; }
        std::cin.ignore();

        if (opt == 1) {
            std::cout << "Ejecutando transferencia de energía (no forzado por defecto)...\n";
            params.F0 = 0.0;
            string out = run_simulation("energy_transfer.csv", s0, 0.0, 120.0, 0.001, integrator);
            std::cout << "Guardado: " << out << "\n";
        }
        else if (opt == 2) {
            std::cout << "Ejecutando barrido resonancia...\n";
            string out = run_resonance_sweep("resonance_sweep.csv", s0, 0.0, 40.0, 0.001, 0.2, 2.0, 40);
            std::cout << "Guardado: " << out << "\n";
        }
        else if (opt == 3) {
            std::cout << "Ejecutando régimen caótico (forzado F0>0)...\n";
            params.gamma = 0.05; params.F0 = 1.5; params.omega_f = 0.66;
            string out = run_simulation("chaos_run.csv", s0, 0.0, 300.0, 0.001, integrator);
            std::cout << "Guardado: " << out << "\n";
        }
        else if (opt == 4) {
            std::cout << "Lanzando Python plot (scripts/plot.py)...\n";
            int rc = system("python3 scripts/plot.py");
            std::cout << "Plot script exit code: " << rc << "\n";
        }
        else if (opt == 5) {
            std::cout << "kappa (actual=" << params.kappa << ") -> ";
            std::string s; std::getline(std::cin, s);
            if (!s.empty()) params.kappa = std::stod(s);
            std::cout << "gamma (actual=" << params.gamma << ") -> "; std::getline(std::cin, s); if (!s.empty()) params.gamma = std::stod(s);
            std::cout << "F0 (actual=" << params.F0 << ") -> "; std::getline(std::cin, s); if (!s.empty()) params.F0 = std::stod(s);
            std::cout << "omega_f (actual=" << params.omega_f << ") -> "; std::getline(std::cin, s); if (!s.empty()) params.omega_f = std::stod(s);
            std::cout << "linear? 0/1 (actual=" << params.linear << ") -> "; std::getline(std::cin, s); if (!s.empty()) params.linear = (std::stoi(s) != 0);
            std::cout << "Parámetros actualizados.\n";
        }
        else if (opt == 6) {
            std::cout << "Selecciona integrador: 1) RK4  2) Euler-Cromer : ";
            int c; if (!(std::cin >> c)) { std::cin.clear(); std::string d; std::getline(std::cin,d); continue; }
            std::cin.ignore();
            integrator = (c==2) ? "EC" : "RK4";
            std::cout << "Integrador: " << integrator << "\n";
        }
        else if (opt == 7) {
            system("zip -r pendulos_results.zip results > /dev/null 2>&1");
            std::cout << "pendulos_results.zip creado.\n";
        }
        else if (opt == 8) {
            break;
        }
        else {
            std::cout << "Opción inválida.\n";
        }
    }
    return 0;
}
