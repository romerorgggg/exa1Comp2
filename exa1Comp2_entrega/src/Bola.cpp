/**
 * @file Bola.cpp
 * @brief Implementación de la clase Bola
 */

#include "../include/Bola.h"

Bola::Bola() : x_(0.0), y_(0.0), vx_(0.0), vy_(0.0), masa_(1.0), radio_(0.1) {}

Bola::Bola(double x, double y, double vx, double vy, double masa, double radio)
    : x_(x), y_(y), vx_(vx), vy_(vy), masa_(masa), radio_(radio) {}

void Bola::Iniciar(double x, double y, double vx, double vy, double masa, double radio) {
  x_ = x;
  y_ = y;
  vx_ = vx;
  vy_ = vy;
  masa_ = masa;
  radio_ = radio;
}

void Bola::MoverseEuler(double dt) {
  x_ += vx_ * dt;
  y_ += vy_ * dt;
}

void Bola::MoverseVerlet(double dt, double ax, double ay) {
  x_ += vx_ * dt + 0.5 * ax * dt * dt;
  y_ += vy_ * dt + 0.5 * ay * dt * dt;
  vx_ += ax * dt;
  vy_ += ay * dt;
}

void Bola::RebotePared(double W, double H) {
  if (x_ - radio_ < 0.0) {
    x_ = radio_;
    vx_ = -vx_;
  }
  if (x_ + radio_ > W) {
    x_ = W - radio_;
    vx_ = -vx_;
  }
  if (y_ - radio_ < 0.0) {
    y_ = radio_;
    vy_ = -vy_;
  }
  if (y_ + radio_ > H) {
    y_ = H - radio_;
    vy_ = -vy_;
  }
}

bool Bola::HayColision(const Bola& otra) const {
  double dx = x_ - otra.x_;
  double dy = y_ - otra.y_;
  double distancia = std::sqrt(dx * dx + dy * dy);
  double suma_radios = radio_ + otra.radio_;
  return distancia < suma_radios;
}

void Bola::Choque(Bola& otra) {
  if (!HayColision(otra)) return;
  
  double dx = otra.x_ - x_;
  double dy = otra.y_ - y_;
  double distancia = std::sqrt(dx * dx + dy * dy);
  
  if (distancia < 1e-10) return;
  
  double nx = dx / distancia;
  double ny = dy / distancia;
  
  double dvx = otra.vx_ - vx_;
  double dvy = otra.vy_ - vy_;
  double dvn = dvx * nx + dvy * ny;
  
  if (dvn > 0) return;
  
  double m1 = masa_;
  double m2 = otra.masa_;
  double impulso = 2.0 * m2 * dvn / (m1 + m2);
  
  vx_ += impulso * nx;
  vy_ += impulso * ny;
  otra.vx_ -= impulso * nx;
  otra.vy_ -= impulso * ny;
  
  double overlap = (radio_ + otra.radio_) - distancia;
  if (overlap > 0) {
    double separacion = overlap / 2.0 + 1e-6;
    x_ -= separacion * nx;
    y_ -= separacion * ny;
    otra.x_ += separacion * nx;
    otra.y_ += separacion * ny;
  }
}

double Bola::EnergiaCinetica() const {
  return 0.5 * masa_ * (vx_ * vx_ + vy_ * vy_);
}

double Bola::Rapidez() const {
  return std::sqrt(vx_ * vx_ + vy_ * vy_);
}