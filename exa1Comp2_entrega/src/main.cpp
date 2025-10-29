/**
 * @file main.cpp
 * @brief Programa principal para simulación de N partículas en una caja
 * @author Tu Nombre
 * @date Octubre 2025
 */

#include <iostream>
#include <string>
#include "../include/Caja.h"

/**
 * @brief Experimento 1: Gas diluido con pocas partículas
 */
void Experimento1GasDiluido() {
  std::cout << "\n=== EXPERIMENTO 1: Gas Diluido ===\n";
  
  Caja caja(10.0, 10.0);  // Caja de 10x10
  caja.InicializarRejilla(25, 1.0, 0.1, 1.0);
  
  double energia_inicial = caja.EnergiaTotal();
  std::cout << "Energía inicial: " << energia_inicial << "\n";
  
  caja.SimularCompleto(10.0, 0.001, 0.05, "results/gas_diluido.dat", true);
  
  double energia_final = caja.EnergiaTotal();
  std::cout << "Energía final: " << energia_final << "\n";
  std::cout << "Conservación: " << (100.0 * energia_final / energia_inicial) << "%\n";
}

/**
 * @brief Experimento 2: Gas denso con muchas partículas
 */
void Experimento2GasDenso() {
  std::cout << "\n=== EXPERIMENTO 2: Gas Denso ===\n";
  
  Caja caja(10.0, 10.0);
  caja.InicializarRejilla(100, 1.0, 0.15, 2.0);
  
  double energia_inicial = caja.EnergiaTotal();
  std::cout << "Energía inicial: " << energia_inicial << "\n";
  
  caja.SimularCompleto(10.0, 0.001, 0.05, "results/gas_denso.dat", true);
  
  double energia_final = caja.EnergiaTotal();
  std::cout << "Energía final: " << energia_final << "\n";
  std::cout << "Conservación: " << (100.0 * energia_final / energia_inicial) << "%\n";
}

/**
 * @brief Experimento 3: Comparación Euler vs Verlet
 */
void Experimento3ComparacionMetodos() {
  std::cout << "\n=== EXPERIMENTO 3: Euler vs Verlet ===\n";
  
  // Simulación con Euler
  std::cout << "\n--- Método de Euler ---\n";
  Caja caja_euler(10.0, 10.0);
  caja_euler.InicializarRejilla(50, 1.0, 0.1, 1.5);
  double e_inicial_euler = caja_euler.EnergiaTotal();
  caja_euler.SimularCompleto(5.0, 0.001, 0.05, "results/euler.dat", false);
  double e_final_euler = caja_euler.EnergiaTotal();
  
  // Simulación con Verlet
  std::cout << "\n--- Método de Velocity-Verlet ---\n";
  Caja caja_verlet(10.0, 10.0);
  caja_verlet.InicializarRejilla(50, 1.0, 0.1, 1.5);
  double e_inicial_verlet = caja_verlet.EnergiaTotal();
  caja_verlet.SimularCompleto(5.0, 0.001, 0.05, "results/verlet.dat", true);
  double e_final_verlet = caja_verlet.EnergiaTotal();
  
  std::cout << "\n--- Comparación ---\n";
  std::cout << "Euler - Conservación: " 
            << (100.0 * e_final_euler / e_inicial_euler) << "%\n";
  std::cout << "Verlet - Conservación: " 
            << (100.0 * e_final_verlet / e_inicial_verlet) << "%\n";
}

/**
 * @brief Experimento 4: Análisis de presión (choques con paredes)
 */
void Experimento4Presion() {
  std::cout << "\n=== EXPERIMENTO 4: Análisis de Presión ===\n";
  
  double anchos[] = {5.0, 10.0, 15.0};
  
  for (double ancho : anchos) {
    std::cout << "\n--- Caja " << ancho << "x" << ancho << " ---\n";
    Caja caja(ancho, ancho);
    caja.InicializarRejilla(50, 1.0, 0.1, 1.5);
    
    double t_simulacion = 5.0;
    caja.SimularCompleto(t_simulacion, 0.001, 0.1, 
                         "results/presion_" + std::to_string(static_cast<int>(ancho)) + ".dat", 
                         true);
    
    int choques = caja.GetNumChoquesPared();
    double tasa_choques = choques / t_simulacion;
    std::cout << "Choques totales: " << choques << "\n";
    std::cout << "Tasa de choques: " << tasa_choques << " choques/s\n";
    std::cout << "Presión relativa: " << tasa_choques << "\n";
  }
}

/**
 * @brief Experimento 5: Distribución de velocidades (Maxwell-Boltzmann)
 */
void Experimento5Velocidades() {
  std::cout << "\n=== EXPERIMENTO 5: Distribución de Velocidades ===\n";
  
  Caja caja(15.0, 15.0);
  caja.InicializarRejilla(200, 1.0, 0.08, 2.0);
  
  std::cout << "Simulando para obtener distribución de velocidades...\n";
  caja.SimularCompleto(10.0, 0.001, 0.02, "results/velocidades.dat", true);
}

/**
 * @brief Función principal
 */
int main(int argc, char* argv[]) {
  std::cout << "========================================\n";
  std::cout << "  Simulación de N partículas en caja  \n";
  std::cout << "  Física Computacional 2 - 2025       \n";
  std::cout << "========================================\n";
  
  // Permitir selección de experimento desde línea de comandos
  int experimento = 0;
  if (argc > 1) {
    experimento = std::stoi(argv[1]);
  }
  
  if (experimento == 0) {
    std::cout << "\nSeleccione el experimento a ejecutar:\n";
    std::cout << "1. Gas diluido (25 partículas)\n";
    std::cout << "2. Gas denso (100 partículas)\n";
    std::cout << "3. Comparación Euler vs Verlet\n";
    std::cout << "4. Análisis de presión\n";
    std::cout << "5. Distribución de velocidades\n";
    std::cout << "6. Ejecutar todos\n";
    std::cout << "\nOpción: ";
    std::cin >> experimento;
  }
  
  switch (experimento) {
    case 1:
      Experimento1GasDiluido();
      break;
    case 2:
      Experimento2GasDenso();
      break;
    case 3:
      Experimento3ComparacionMetodos();
      break;
    case 4:
      Experimento4Presion();
      break;
    case 5:
      Experimento5Velocidades();
      break;
    case 6:
      Experimento1GasDiluido();
      Experimento2GasDenso();
      Experimento3ComparacionMetodos();
      Experimento4Presion();
      Experimento5Velocidades();
      break;
    default:
      std::cerr << "Opción inválida.\n";
      return 1;
  }
  
  std::cout << "\n========================================\n";
  std::cout << "  Simulación completada exitosamente  \n";
  std::cout << "  Revise la carpeta results/          \n";
  std::cout << "========================================\n";
  
  return 0;
}