#!/usr/bin/env python3
"""
Genera solo la gráfica de comparación de métodos
"""

import numpy as np
import matplotlib.pyplot as plt

def cargar_datos(archivo):
    try:
        datos = np.loadtxt(archivo)
        return datos
    except Exception as e:
        print(f"Error al cargar {archivo}: {e}")
        return None

def calcular_energia_total(datos, masa=1.0):
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

print("Generando comparación de métodos...")

archivos = [
    ("results/euler.dat", "Euler", "blue"),
    ("results/verlet.dat", "Velocity-Verlet", "red")
]

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

for archivo, metodo, color in archivos:
    try:
        print(f"Procesando {archivo}...")
        datos = cargar_datos(archivo)
        
        if datos is None:
            print(f"  No se pudo cargar {archivo}")
            continue
            
        t, energia = calcular_energia_total(datos)
        E0 = energia[0]
        
        energia_norm = energia / E0
        ax1.plot(t, energia_norm, color=color, linewidth=2, 
                label=metodo, alpha=0.7)
        
        error_rel = (energia - E0) / E0 * 100
        ax2.plot(t, error_rel, color=color, linewidth=2, 
                label=metodo, alpha=0.7)
        
        print(f"  OK - {metodo} procesado")
        
    except Exception as e:
        print(f"  Error procesando {archivo}: {e}")

ax1.axhline(y=1.0, color='k', linestyle='--', linewidth=1, alpha=0.5)
ax1.set_xlabel('Tiempo (s)', fontsize=12)
ax1.set_ylabel('Energia normalizada (E/E0)', fontsize=12)
ax1.set_title('Comparacion de conservacion de energia: Euler vs Verlet', fontsize=14)
ax1.legend(fontsize=11)
ax1.grid(True, alpha=0.3)

ax2.axhline(y=0, color='k', linestyle='--', linewidth=1, alpha=0.5)
ax2.set_xlabel('Tiempo (s)', fontsize=12)
ax2.set_ylabel('Error relativo (%)', fontsize=12)
ax2.set_title('Error en la conservacion de energia', fontsize=14)
ax2.legend(fontsize=11)
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("results/comparacion_metodos.png", dpi=150, bbox_inches='tight')
print("\nGuardado: results/comparacion_metodos.png")
plt.close()

print("Completado!")
