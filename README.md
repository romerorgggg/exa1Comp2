
Desarrollo del parcial 1 del curso Computacional 2
\#  Péndulos Acoplados Amortiguados y Forzados



\*\*Curso:\*\* Física Computacional II





\##  Descripción del Proyecto



Este proyecto implementa la simulación numérica de \*\*dos péndulos acoplados con amortiguamiento, forzamiento externo y una interacción no lineal cuadrática\*\*. El sistema se resuelve usando \*\*Runge-Kutta de 4º orden (RK4)\*\* o \*\*Euler-Cromer\*\*, permitiendo al usuario configurar todos los parámetros de forma interactiva.



El software, desarrollado en C++17, permite explorar fenómenos como la \*\*transferencia de energía\*\*, \*\*sincronización\*\*, \*\*resonancia\*\* y \*\*caos\*\*.



\##  Modelo Físico



El sistema está gobernado por las ecuaciones:

\\\[

\\begin{cases}

\\ddot{\\theta}\_1 + \\gamma \\dot{\\theta}\_1 + \\dfrac{g}{l}\\sin(\\theta\_1) + \\kappa(\\theta\_1 - \\theta\_2)^2 = F\_0 \\cos(\\omega t), \\\\

\\ddot{\\theta}\_2 + \\gamma \\dot{\\theta}\_2 + \\dfrac{g}{l}\\sin(\\theta\_2) + \\kappa(\\theta\_2 - \\theta\_1)^2 = F\_0 \\cos(\\omega t).

\\end{cases}

\\]



\##  Estructura del Proyecto

pendulos_proyecto/
+-- include/
|   +-- Pendulo.h
+-- src/
|   +-- Pendulo.cpp
|   +-- main.cpp
+-- scripts/
|   +-- plot.py
+-- results/
+-- documents/
|   +-- analysis.tex
|   +-- flowchart.png
+-- Makefile
+-- Doxyfile
+-- README.md\##  Compilación y Uso



\### 1. Compilar el proyecto

```bash

make

\###2. Ejecutar la simulación

\###El programa te guiará para introducir los parámetros.

make run

\###Los datos se guardan en results/simulation\_data.csv.

\###3. Generar gráficos

make plot

\###Crea theta\_vs\_t.png, phase\_space.png y energy.png en results/.

\###4. Generar documentación

make doc

\###Abre docs/html/index.html.

\###5. Generar reporte PDF

\###Edita documents/analysis.tex y compila:

make report



---

\#### \*\*ARCHIVO: `include/Pendulo.h`\*\*

```cpp

\#ifndef PENDULO\_H

\#define PENDULO\_H



\#include <cmath>



/\*\*

&nbsp;\* @struct Params

&nbsp;\* @brief Almacena los parámetros físicos y de simulación del sistema.

&nbsp;\*/

struct Params {

&nbsp;   // Parámetros básicos

&nbsp;   double g = 9.81;    ///< Aceleración gravitacional (m/s^2)

&nbsp;   double l = 9.81;    ///< Longitud del péndulo (m)

&nbsp;   double m = 1.0;     ///< Masa del péndulo (kg)

&nbsp;   double kappa = 0.5; ///< Constante de acoplamiento no lineal



&nbsp;   // Parámetros del modelo extendido

&nbsp;   double gamma = 0.1; ///< Coeficiente de amortiguamiento

&nbsp;   double F0 = 0.0;    ///< Amplitud del forzamiento

&nbsp;   double omega = 1.0; ///< Frecuencia angular del forzamiento

};



/\*\*

&nbsp;\* @class Pendulo

&nbsp;\* @brief Representa un péndulo individual en el sistema acoplado.

&nbsp;\*/

class Pendulo {

private:

&nbsp;   double theta\_; ///< Ángulo actual en radianes.

&nbsp;   double omega\_; ///< Velocidad angular actual en rad/s.



public:

&nbsp;   /\*\*

&nbsp;    \* @brief Constructor de la clase Pendulo.

&nbsp;    \* @param theta0 Ángulo inicial.

&nbsp;    \* @param omega0 Velocidad angular inicial.

&nbsp;    \*/

&nbsp;   Pendulo(double theta0, double omega0);



&nbsp;   // Getters y Setters

&nbsp;   double get\_theta() const { return theta\_; }

&nbsp;   double get\_omega() const { return omega\_; }

&nbsp;   void set\_theta(double new\_theta) { theta\_ = new\_theta; }

&nbsp;   void set\_omega(double new\_omega) { omega\_ = new\_omega; }



&nbsp;   /\*\*

&nbsp;    \* @brief Calcula la energía total de este péndulo (cinética + potencial gravitacional).

&nbsp;    \* @param p Estructura con los parámetros físicos.

&nbsp;    \* @return La energía individual del péndulo.

&nbsp;    \*/

&nbsp;   double individual\_energy(const Params\& p) const;

};



/\*\*

&nbsp;\* @brief Calcula la aceleración angular de un péndulo en un instante dado.

&nbsp;\* @param theta\_this Ángulo del péndulo para el cual se calcula la aceleración.

&nbsp;\* @param omega\_this Velocidad angular del mismo péndulo.

&nbsp;\* @param theta\_other Ángulo del otro péndulo (para el acoplamiento).

&nbsp;\* @param t Tiempo actual (para el forzamiento).

&nbsp;\* @param p Estructura con los parámetros físicos del sistema.

&nbsp;\* @return La aceleración angular (theta\_ddot).

&nbsp;\*/

double calculate\_accel(double theta\_this, double omega\_this, double theta\_other, double t, const Params\& p);



\#endif // PENDULO\_H

>>>>>>> ae61d9ba (Proyecto completo de péndulos acoplados avanzados con documentación y simulación)
