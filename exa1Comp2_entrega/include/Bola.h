/**
 * @file Bola.h
 * @brief Definición de la clase Bola para simulación de partículas
 * @author Tu Nombre
 * @date Octubre 2025
 */

#ifndef BOLA_H
#define BOLA_H

#include <cmath>
#include <iostream>

/**
 * @class Bola
 * @brief Representa una partícula esférica con posición, velocidad y propiedades físicas
 */
class Bola {
 private:
  double x_;       ///< Posición en x
  double y_;       ///< Posición en y
  double vx_;      ///< Velocidad en x
  double vy_;      ///< Velocidad en y
  double masa_;    ///< Masa de la partícula
  double radio_;   ///< Radio de la partícula

 public:
  Bola();
  Bola(double x, double y, double vx, double vy, double masa, double radio);
  void Iniciar(double x, double y, double vx, double vy, double masa, double radio);
  void MoverseEuler(double dt);
  void MoverseVerlet(double dt, double ax = 0.0, double ay = 0.0);
  void RebotePared(double W, double H);
  bool HayColision(const Bola& otra) const;
  void Choque(Bola& otra);
  double EnergiaCinetica() const;
  double Rapidez() const;

  // Getters
  double GetX() const { return x_; }
  double GetY() const { return y_; }
  double GetVx() const { return vx_; }
  double GetVy() const { return vy_; }
  double GetMasa() const { return masa_; }
  double GetRadio() const { return radio_; }

  // Setters
  void SetVx(double vx) { vx_ = vx; }
  void SetVy(double vy) { vy_ = vy; }
};

#endif  // BOLA_H