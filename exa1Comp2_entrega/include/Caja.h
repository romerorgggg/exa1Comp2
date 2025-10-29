/**
 * @file Caja.h
 * @brief Definición de la clase Caja para gestionar el sistema de partículas
 * @author Tu Nombre
 * @date Octubre 2025
 */

#ifndef CAJA_H
#define CAJA_H

#include <vector>
#include <fstream>
#include <string>
#include <random>
#include "Bola.h"

/**
 * @class Caja
 * @brief Gestiona un sistema de N partículas en una caja rectangular
 */
class Caja {
 private:
  double ancho_;
  double alto_;
  std::vector<Bola> bolas_;
  int num_choques_pared_;
  std::mt19937 generador_;

 public:
  Caja(double ancho, double alto);
  void InicializarRejilla(int N, double masa, double radio, double v_max);
  void InicializarAleatorio(int N, double masa, double radio, double v_max);
  void EvolucionarEuler(double dt);
  void EvolucionarVerlet(double dt);
  void DetectarColisiones();
  double EnergiaTotal() const;
  void GuardarEstado(std::ofstream& archivo, double tiempo) const;
  void SimularCompleto(double t_final, double dt, double dt_salida,
                       const std::string& nombre_archivo, bool usar_verlet = true);
  int GetNumChoquesPared() const { return num_choques_pared_; }
  int GetNumBolas() const { return bolas_.size(); }
  void ResetearChoques() { num_choques_pared_ = 0; }
};

#endif  // CAJA_H