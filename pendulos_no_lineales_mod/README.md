# Simulación de Péndulos Acoplados con Interacción No Lineal Cuadrática

Este proyecto implementa la simulación de **dos péndulos acoplados** con una **interacción cuadrática no lineal**, usando integración numérica con **Runge–Kutta 4 (RK4)** en **C++**.  
Forma parte del *Examen 1 de Física Computacional 2* (Profesor John H. Díaz, 2025).

---

## 🧠 Modelo físico

El sistema está descrito por las ecuaciones diferenciales:

\[
\begin{cases}
\ddot{\theta}_1 + \dfrac{g}{l}\sin\theta_1 + \kappa(\theta_1 - \theta_2)^2 = 0, \\
\ddot{\theta}_2 + \dfrac{g}{l}\sin\theta_2 + \kappa(\theta_2 - \theta_1)^2 = 0.
\end{cases}
\]

Donde:
- \( g \): gravedad
- \( l \): longitud del péndulo
- \( \kappa \): coeficiente de acoplamiento cuadrático

---

## ⚙️ Estructura del proyecto

pendulos_no_lineales/
├─ include/
│ └─ Pendulo.h
├─ src/
│ ├─ Pendulo.cpp
│ └─ main.cpp
├─ scripts/
│ └─ plot.py
├─ results/
├─ Makefile
└─ README.md

---

## 🧩 Compilación y ejecución

```bash
make
./bin/pendulos

