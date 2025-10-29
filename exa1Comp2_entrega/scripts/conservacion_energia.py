#!/usr/bin/env python3
"""
Análisis de conservación de energía
Física Computacional 2 - 2025
"""

import numpy as np
import matplotlib.pyplot as plt

def cargar_datos(archivo):
    """Carga los datos de la simulación"""
    try:
        datos = np.loadtxt(archivo)
        return datos
    except Exception as e:
        print(f"Error al cargar {archivo}: {e}")
        return None

def calcular_energia_total(datos, masa=1.0):
    """Calcula la energía cinética total en cada instante de tiempo"""
    num_cols = datos.shape[1]
    num_particulas = (num_cols - 1) // 4
    
    t = datos[:, 0]
    energia = np.zeros(len(t))
    
    for i in range(num_particulas):
        col_vx = 3 + 4*i
        col_vy = 4 + 4*i
        vx = datos[:, col_vx]
        vy = datos[:, col_vy]
        
        energia += 0.5 * masa * (vx**2 + vy**2)
    
    return t, energia

def analizar_conservacion(archivo, titulo="Conservación de Energía"):
    """Analiza la conservación de energía en la simulación"""
    datos = cargar_datos(archivo)
    if datos is None:
        return None
    
    t, energia = calcular_energia_total(datos)
    
    E0 = energia[0]
    E_final = energia[-1]
    E_media = np.mean(energia)
    E_std = np.std(energia)
    
    error_relativo = abs(E_final - E0) / E0 * 100
    fluctuacion = E_std / E_media * 100
    
    print(f"\n=== {titulo} ===")
    print(f"Energía inicial: {E0:.6f}")
    print(f"Energía final: {E_final:.6f}")
    print(f"Energía media: {E_media:.6f}")
    print(f"Desv. estándar: {E_std:.6f}")
    print(f"Error relativo: {error_relativo:.4f}%")
    print(f"Fluctuación: {fluctuacion:.4f}%")
    
    return t, energia, E0, error_relativo

def graficar_energia(archivo, titulo="Conservación de Energía"):
    """Grafica la evolución temporal de la energía"""
    resultado = analizar_conservacion(archivo, titulo)
    if resultado is None:
        return None
    
    t, energia, E0, error = resultado
    
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
    
    # Energía vs tiempo
    ax1.plot(t, energia, 'b-', linewidth=1, alpha=0.7)
    ax1.axhline(y=E0, color='r', linestyle='--', linewidth=2, label=f'E₀ = {E0:.3f}')
    ax1.set_xlabel('Tiempo (s)')
    ax1.set_ylabel('Energía Cinética Total')
    ax1.set_title(f'{titulo}\nError relativo: {error:.4f}%')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Error relativo vs tiempo
    error_rel = (energia - E0) / E0 * 100
    ax2.plot(t, error_rel, 'r-', linewidth=1)
    ax2.axhline(y=0, color='k', linestyle='--', linewidth=1)
    ax2.set_xlabel('Tiempo (s)')
    ax2.set_ylabel('Error relativo (%)')
    ax2.set_title('Error en la conservación de energía')
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    return fig

def comparar_metodos():
    """Compara conservación de energía entre Euler y Verlet"""
    archivos = [
        ("results/euler.dat", "Euler"),
        ("results/verlet.dat", "Velocity-Verlet")
    ]
    
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
    colores = ['blue', 'red']
    
    for i, (archivo, metodo) in enumerate(archivos):
        try:
            datos = cargar_datos(archivo)
            if datos is None:
                print(f"No se pudo cargar el archivo {archivo}.")
                continue

            t, energia = calcular_energia_total(datos)
            E0 = energia[0]
            
            # Energía normalizada
            energia_norm = energia / E0
            ax1.plot(t, energia_norm, color=colores[i], linewidth=2, 
                     label=metodo, alpha=0.7)
            
            # Error relativo (%)
            error_rel = (energia - E0) / E0 * 100
            ax2.plot(t, error_rel, color=colores[i], linewidth=2, 
                     label=metodo, alpha=0.7)
        
        except Exception as e:
            print(f"Error al procesar {archivo}: {e}")
            continue
    
    # Configuración de gráficos
    ax1.set_xlabel('Tiempo (s)')
    ax1.set_ylabel('Energía Normalizada')
    ax1.set_title('Comparación de Conservación de Energía')
    ax1.legend()
    ax1.grid(True, alpha=0.3)

    ax2.set_xlabel('Tiempo (s)')
    ax2.set_ylabel('Error Relativo (%)')
    ax2.set_title('Error Relativo en la Conservación de Energía')
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    return fig
