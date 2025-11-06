#ifndef PENDULO_H
#define PENDULO_H

#include <cmath>

/**
 * @struct Params
 * @brief Almacena los parámetros físicos y de simulación del sistema.
 */
struct Params {
    // Parámetros básicos
    double g = 9.81;    ///< Aceleración gravitacional (m/s^2)
    double l = 9.81;    ///< Longitud del péndulo (m)
    double m = 1.0;     ///< Masa del péndulo (kg)
    double kappa = 0.5; ///< Constante de acoplamiento no lineal

    // Parámetros del modelo extendido
    double gamma = 0.1; ///< Coeficiente de amortiguamiento
    double F0 = 0.0;    ///< Amplitud del forzamiento
    double omega = 1.0; ///< Frecuencia angular del forzamiento
};

/**
 * @class Pendulo
 * @brief Representa un péndulo individual en el sistema acoplado.
 */
class Pendulo {
private:
    // --- ESTAS LÍNEAS FALTABAN ---
    double theta_; ///< Ángulo actual en radianes.
    double omega_; ///< Velocidad angular actual en rad/s.

public:
    /**
     * @brief Constructor de la clase Pendulo.
     * @param theta0 Ángulo inicial.
     * @param omega0 Velocidad angular inicial.
     */
    Pendulo(double theta0, double omega0);

    // Getters y Setters
    double get_theta() const { return theta_; }
    double get_omega() const { return omega_; }
    void set_theta(double new_theta) { theta_ = new_theta; }
    void set_omega(double new_omega) { omega_ = new_omega; }

    /**
     * @brief Calcula la energía total de este péndulo (cinética + potencial gravitacional).
     * @param p Estructura con los parámetros físicos.
     * @return La energía individual del péndulo.
     */
    // --- ESTA DECLARACIÓN FALTABA ---
    double individual_energy(const Params& p) const;
};

/**
 * @brief Calcula la aceleración angular de un péndulo en un instante dado.
 * @param theta_this Ángulo del péndulo para el cual se calcula la aceleración.
 * @param omega_this Velocidad angular del mismo péndulo.
 * @param theta_other Ángulo del otro péndulo (para el acoplamiento).
 * @param t Tiempo actual (para el forzamiento).
 * @param p Estructura con los parámetros físicos del sistema.
 * @return La aceleración angular (theta_ddot).
 */
double calculate_accel(double theta_this, double omega_this, double theta_other, double t, const Params& p);

#endif // PENDULO_H