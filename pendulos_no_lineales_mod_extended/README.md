# 🪐 Péndulos Acoplados con Interacción No Lineal Cuadrática

**Autor:** [Tu nombre]  
**Curso:** Física Computacional II — Universidad Distrital  
**Profesor:** John Hernán Díaz  
**Fecha:** Octubre 2025  

---

## 🧠 Descripción del Proyecto

Este proyecto implementa la simulación numérica de **dos péndulos acoplados con una interacción no lineal cuadrática**, resolviendo el sistema acoplado de ecuaciones diferenciales mediante el método de **Runge–Kutta de cuarto orden (RK4)**.

El trabajo sigue las indicaciones del *Examen 1 – Física Computacional II* y de la guía *Sistemas acoplados no lineales*, desarrollando un software científico modular en **C++17** con postprocesado en **Python (Matplotlib + Pandas)**.

---

## ⚙️ Modelo Físico

El sistema está gobernado por las ecuaciones:

\[
\begin{cases}
\ddot{\theta}_1 + \dfrac{g}{l}\sin(\theta_1) + \kappa(\theta_1 - \theta_2)^2 = 0, \\
\ddot{\theta}_2 + \dfrac{g}{l}\sin(\theta_2) + \kappa(\theta_2 - \theta_1)^2 = 0,
\end{cases}
\]

donde:
- \( \theta_i \): ángulo del péndulo *i*  
- \( g \): aceleración gravitacional  
- \( l \): longitud del péndulo  
- \( \kappa \): constante de acoplamiento no lineal  

El término \((\theta_1 - \theta_2)^2\) introduce una **interacción cuadrática**, responsable de comportamientos no lineales y transferencia de energía entre los péndulos.

---

## 🧮 Método Numérico

Se usa **Runge–Kutta 4 (RK4)** para integrar el sistema:

\[
\mathbf{y} = (\theta_1, \omega_1, \theta_2, \omega_2)
\]

con paso de tiempo `dt` ajustable.  
La energía total del sistema se calcula como:

\[
E_T = T_1 + T_2 + V_{g1} + V_{g2} + V_c,
\]
donde  
\[
V_c = \frac{\kappa}{3}(\theta_1 - \theta_2)^3.
\]

---

## 🧩 Estructura del Proyecto

pendulos_no_lineales/
├─ include/
│ └─ Pendulo.h # Clase Pendulo: atributos, parámetros, funciones auxiliares
├─ src/
│ ├─ Pendulo.cpp # Implementación de la clase Pendulo
│ └─ main.cpp # Simulación principal e integración RK4
├─ scripts/
│ └─ plot.py # Post-procesamiento y graficación
├─ results/ # Salidas de simulación (CSV, PNG)
├─ Makefile # Compilación modular
└─ README.md # Documentación general del proyecto


---

## 🧰 Requisitos

- **Compilador:** `g++` con soporte C++17  
- **Python 3** con librerías:
  - `numpy`
  - `pandas`
  - `matplotlib`

Puedes instalarlas (en un entorno virtual) con:
```bash
python3 -m venv venv
source venv/bin/activate
pip install numpy pandas matplotlib
 

🧱 Compilación y Ejecución

1️⃣ Compila el proyecto:

make


2️⃣ Ejecuta la simulación:

./bin/pendulos


Esto genera el archivo:

results/results.csv


3️⃣ Graficar los resultados:

python3 scripts/plot.py results/results.csv


Genera:

results/theta_vs_t.png — evolución temporal de θ₁ y θ₂

results/phase_space.png — espacios fase (θ, ω)

results/energy.png — energía total vs tiempo
