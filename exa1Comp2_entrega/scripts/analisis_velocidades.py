#!/usr/bin/env python3
"""
Análisis de distribución de velocidades y comparación con Maxwell-Boltzmann
Física Computacional 2 - 2025
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import maxwell

def cargar_datos(archivo):
    """Carga los datos de la simulación"""
    try:
        datos = np.loadtxt(archivo)
        return datos
    except Exception as e:
        print(f"Error al cargar {archivo}: {e}")
        return None

def extraer_velocidades(datos, num_particulas):
    """Extrae las velocidades vx, vy de todas las partículas"""
    vx = []
    vy = []
    
    for i in range(num_particulas):
        col_vx = 3 + 4*i
        col_vy = 4 + 4*i
        vx.append(datos[:, col_vx])
        vy.append(datos[:, col_vy])
    
    return np.array(vx), np.array(vy)

def calcular_rapideces(vx, vy):
    """Calcula las rapideces |v| de todas las partículas en todos los tiempos"""
    rapideces = np.sqrt(vx**2 + vy**2)
    return rapideces.flatten()

def maxwell_boltzmann_2d(v, temperatura):
    """
    Distribución de Maxwell-Boltzmann en 2D
    P(v) = (m/(kT)) * v * exp(-m*v^2/(2kT))
    Para simplificar, usamos m=1 y k=1
    """
    return (v / temperatura) * np.exp(-v**2 / (2 * temperatura))

def analizar_distribucion(archivo, titulo="Distribución de Velocidades"):
    """Analiza la distribución de velocidades y compara con Maxwell-Boltzmann"""
    datos = cargar_datos(archivo)
    if datos is None:
        return
    
    num_cols = datos.shape[1]
    num_particulas = (num_cols - 1) // 4
    
    print(f"\n=== {titulo} ===")
    print(f"Número de partículas: {num_particulas}")
    print(f"Número de frames: {len(datos)}")
    
    # Extraer velocidades
    vx, vy = extraer_velocidades(datos, num_particulas)
    rapideces = calcular_rapideces(vx, vy)
    
    # Estadísticas
    v_media = np.mean(rapideces)
    v_std = np.std(rapideces)
    v_max = np.max(rapideces)
    
    print(f"Velocidad media: {v_media:.4f}")
    print(f"Desviación estándar: {v_std:.4f}")
    print(f"Velocidad máxima: {v_max:.4f}")
    
    # Estimar temperatura efectiva
    # Para 2D: <v^2> = 2*k*T/m, con m=1, k=1
    temperatura = np.mean(rapideces**2) / 2.0
    print(f"Temperatura efectiva: {temperatura:.4f}")
    
    # Crear figura con dos subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    # Histograma de rapideces
    bins = 50
    counts, bin_edges, _ = ax1.hist(rapideces, bins=bins, density=True, 
                                     alpha=0.7, color='blue', 
                                     label='Simulación', edgecolor='black')
    
    # Curva teórica de Maxwell-Boltzmann
    v_teorico = np.linspace(0, v_max * 1.1, 1000)
    prob_teorica = maxwell_boltzmann_2d(v_teorico, temperatura)
    ax1.plot(v_teorico, prob_teorica, 'r-', linewidth=2, 
            label='Maxwell-Boltzmann 2D')
    
    ax1.set_xlabel('Rapidez |v|')
    ax1.set_ylabel('Densidad de probabilidad')
    ax1.set_title(f'{titulo}\n(T efectiva = {temperatura:.3f})')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Histograma de componentes vx y vy
    vx_flat = vx.flatten()
    vy_flat = vy.flatten()
    
    ax2.hist(vx_flat, bins=bins, density=True, alpha=0.6, 
            color='red', label='vx', edgecolor='black')
    ax2.hist(vy_flat, bins=bins, density=True, alpha=0.6, 
            color='green', label='vy', edgecolor='black')
    
    # Gaussiana teórica para componentes
    v_comp = np.linspace(min(vx_flat.min(), vy_flat.min()), 
                         max(vx_flat.max(), vy_flat.max()), 1000)
    sigma = np.sqrt(temperatura)
    gaussian = (1.0 / (np.sqrt(2 * np.pi) * sigma)) * np.exp(-v_comp**2 / (2 * sigma**2))
    ax2.plot(v_comp, gaussian, 'k--', linewidth=2, label='Gaussiana teórica')
    
    ax2.set_xlabel('Componente de velocidad')
    ax2.set_ylabel('Densidad de probabilidad')
    ax2.set_title('Distribución de componentes')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    return fig, temperatura

def comparar_distribuciones():
    """Compara distribuciones de diferentes experimentos"""
    experimentos = [
        ("results/gas_diluido.dat", "Gas Diluido"),
        ("results/gas_denso.dat", "Gas Denso"),
        ("results/velocidades.dat", "200 Partículas")
    ]
    
    fig, ax = plt.subplots(figsize=(10, 7))
    colores = ['blue', 'red', 'green']
    
    for i, (archivo, nombre) in enumerate(experimentos):
        try:
            datos = cargar_datos(archivo)
            if datos is None:
                continue
            
            num_cols = datos.shape[1]
            num_particulas = (num_cols - 1) // 4
            
            vx, vy = extraer_velocidades(datos, num_particulas)
            rapideces = calcular_rapideces(vx, vy)
            
            # Histograma normalizado
            ax.hist(rapideces, bins=40, density=True, alpha=0.5, 
                   color=colores[i], label=nombre, edgecolor='black')
            
        except Exception as e:
            print(f"Error procesando {archivo}: {e}")
    
    ax.set_xlabel('Rapidez |v|')
    ax.set_ylabel('Densidad de probabilidad')
    ax.set_title('Comparación de distribuciones de velocidades')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    return fig

if __name__ == "__main__":
    print("=== Análisis de Distribución de Velocidades ===\n")
    
    # Analizar cada experimento
    experimentos = [
        ("results/gas_diluido.dat", "Gas Diluido"),
        ("results/gas_denso.dat", "Gas Denso"),
        ("results/velocidades.dat", "200 Partículas")
    ]
    
    for archivo, nombre in experimentos:
        try:
            fig, temp = analizar_distribucion(archivo, nombre)
            plt.savefig(f"results/dist_velocidades_{nombre.lower().replace(' ', '_')}.png",
                       dpi=150, bbox_inches='tight')
            print(f"Guardado: dist_velocidades_{nombre.lower().replace(' ', '_')}.png")
            plt.close()
        except Exception as e:
            print(f"Error procesando {archivo}: {e}")
    
    # Comparación entre experimentos
    try:
        fig_comp = comparar_distribuciones()
        plt.savefig("results/comparacion_velocidades.png", 
                   dpi=150, bbox_inches='tight')
        print("\nGuardado: comparacion_velocidades.png")
        plt.close()
    except Exception as e:
        print(f"Error en comparación: {e}")
    
    print("\n=== Análisis completado ===")
    