/**
 * @file Caja.cpp
 * @brief Implementación de la clase Caja
 */

#include "../include/Caja.h"
#include <iostream>
#include <cmath>

Caja::Caja(double ancho, double alto) 
    : ancho_(ancho), alto_(alto), num_choques_pared_(0) {
  std::random_device rd;
  generador_.seed(rd());
}

void Caja::InicializarRejilla(int N, double masa, double radio, double v_max) {
  bolas_.clear();
  
  int cols = static_cast<int>(std::ceil(std::sqrt(N)));
  int filas = static_cast<int>(std::ceil(static_cast<double>(N) / cols));
  
  double espaciado_x = ancho_ / (cols + 1);
  double espaciado_y = alto_ / (filas + 1);
  
  std::uniform_real_distribution<double> dist_vel(-v_max, v_max);
  
  int contador = 0;
  for (int i = 0; i < filas && contador < N; ++i) {
    for (int j = 0; j < cols && contador < N; ++j) {
      double x = (j + 1) * espaciado_x;
      double y = (i + 1) * espaciado_y;
      double vx = dist_vel(generador_);
      double vy = dist_vel(generador_);
      
      bolas_.push_back(Bola(x, y, vx, vy, masa, radio));
      contador++;
    }
  }
  
  std::cout << "Inicializadas " << bolas_.size() << " particulas en rejilla.\n";
}

void Caja::InicializarAleatorio(int N, double masa, double radio, double v_max) {
  bolas_.clear();
  
  std::uniform_real_distribution<double> dist_x(radio, ancho_ - radio);
  std::uniform_real_distribution<double> dist_y(radio, alto_ - radio);
  std::uniform_real_distribution<double> dist_vel(-v_max, v_max);
  
  int intentos_max = 10000;
  
  for (int i = 0; i < N; ++i) {
    bool posicion_valida = false;
    double x, y;
    int intentos = 0;
    
    while (!posicion_valida && intentos < intentos_max) {
      x = dist_x(generador_);
      y = dist_y(generador_);
      
      posicion_valida = true;
      Bola temp(x, y, 0, 0, masa, radio);
      
      for (const auto& bola : bolas_) {
        if (temp.HayColision(bola)) {
          posicion_valida = false;
          break;
        }
      }
      intentos++;
    }
    
    if (posicion_valida) {
      double vx = dist_vel(generador_);
      double vy = dist_vel(generador_);
      bolas_.push_back(Bola(x, y, vx, vy, masa, radio));
    } else {
      std::cerr << "Advertencia: No se pudo colocar particula " << i << "\n";
    }
  }
  
  std::cout << "Inicializadas " << bolas_.size() << " particulas aleatoriamente.\n";
}

void Caja::EvolucionarEuler(double dt) {
  // Mover todas las particulas
  for (auto& bola : bolas_) {
    bola.MoverseEuler(dt);
  }
  
  // Manejar choques con paredes
  int choques_este_paso = 0;
  for (auto& bola : bolas_) {
    double x_ant = bola.GetX();
    double y_ant = bola.GetY();
    bola.RebotePared(ancho_, alto_);
    
    // Contar si hubo rebote
    if (bola.GetX() != x_ant || bola.GetY() != y_ant) {
      choques_este_paso++;
    }
  }
  num_choques_pared_ += choques_este_paso;
  
  // Detectar colisiones entre particulas
  DetectarColisiones();
}

void Caja::EvolucionarVerlet(double dt) {
  // Mover todas las particulas (sin aceleracion externa)
  for (auto& bola : bolas_) {
    bola.MoverseVerlet(dt, 0.0, 0.0);
  }
  
  // Manejar choques con paredes
  int choques_este_paso = 0;
  for (auto& bola : bolas_) {
    double vx_ant = bola.GetVx();
    double vy_ant = bola.GetVy();
    bola.RebotePared(ancho_, alto_);
    
    // Contar si hubo rebote (cambio de velocidad)
    if (bola.GetVx() != vx_ant || bola.GetVy() != vy_ant) {
      choques_este_paso++;
    }
  }
  num_choques_pared_ += choques_este_paso;
  
  // Detectar colisiones entre particulas
  DetectarColisiones();
}

void Caja::DetectarColisiones() {
  int N = bolas_.size();
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      if (bolas_[i].HayColision(bolas_[j])) {
        bolas_[i].Choque(bolas_[j]);
      }
    }
  }
}

double Caja::EnergiaTotal() const {
  double energia = 0.0;
  for (const auto& bola : bolas_) {
    energia += bola.EnergiaCinetica();
  }
  return energia;
}

void Caja::GuardarEstado(std::ofstream& archivo, double tiempo) const {
  archivo << tiempo;
  for (const auto& bola : bolas_) {
    archivo << " " << bola.GetX() << " " << bola.GetY()
            << " " << bola.GetVx() << " " << bola.GetVy();
  }
  archivo << "\n";
}

void Caja::SimularCompleto(double t_final, double dt, double dt_salida,
                           const std::string& nombre_archivo, bool usar_verlet) {
  std::ofstream archivo(nombre_archivo);
  
  if (!archivo.is_open()) {
    std::cerr << "Error al abrir archivo: " << nombre_archivo << "\n";
    return;
  }
  
  // Escribir encabezado
  archivo << "# t";
  for (size_t i = 0; i < bolas_.size(); ++i) {
    archivo << " x" << i << " y" << i << " vx" << i << " vy" << i;
  }
  archivo << "\n";
  
  double t = 0.0;
  double t_siguiente_salida = 0.0;
  
  std::cout << "Iniciando simulacion...\n";
  std::cout << "Metodo: " << (usar_verlet ? "Velocity-Verlet" : "Euler") << "\n";
  std::cout << "Tiempo final: " << t_final << " s\n";
  std::cout << "Paso de tiempo: " << dt << " s\n";
  
  int pasos = 0;
  while (t < t_final) {
    // Guardar estado si corresponde
    if (t >= t_siguiente_salida) {
      GuardarEstado(archivo, t);
      t_siguiente_salida += dt_salida;
    }
    
    // Evolucionar el sistema
    if (usar_verlet) {
      EvolucionarVerlet(dt);
    } else {
      EvolucionarEuler(dt);
    }
    
    t += dt;
    pasos++;
    
    // Progreso cada 10% de la simulacion
    if (pasos % static_cast<int>(t_final / (10 * dt)) == 0) {
      std::cout << "Progreso: " << (100.0 * t / t_final) << "%\n";
    }
  }
  
  // Guardar estado final
  GuardarEstado(archivo, t);
  
  archivo.close();
  std::cout << "Simulacion completada.\n";
  std::cout << "Total de choques con paredes: " << num_choques_pared_ << "\n";
  std::cout << "Energia final: " << EnergiaTotal() << "\n";
}
