#!/usr/bin/env python3
"""
Visualización y animación de partículas en la caja
Física Computacional 2 - 2025
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.patches import Circle
import sys

def cargar_datos(archivo):
    """Carga los datos de la simulación"""
    try:
        datos = np.loadtxt(archivo)
        return datos
    except Exception as e:
        print(f"Error al cargar {archivo}: {e}")
        return None

def extraer_posiciones(datos, num_particulas):
    """Extrae posiciones x, y de todas las partículas"""
    t = datos[:, 0]
    x = []
    y = []
    
    for i in range(num_particulas):
        col_x = 1 + 4*i
        col_y = 2 + 4*i
        x.append(datos[:, col_x])
        y.append(datos[:, col_y])
    
    return t, np.array(x), np.array(y)

def animar_particulas(archivo, ancho=10.0, alto=10.0, radio=0.1):
    """Crea animación de las partículas"""
    datos = cargar_datos(archivo)
    if datos is None:
        return
    
    # Determinar número de partículas
    num_cols = datos.shape[1]
    num_particulas = (num_cols - 1) // 4
    
    print(f"Número de partículas: {num_particulas}")
    
    t, x, y = extraer_posiciones(datos, num_particulas)
    num_frames = len(t)
    
    # Configurar figura
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_xlim(0, ancho)
    ax.set_ylim(0, alto)
    ax.set_aspect('equal')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_title('Simulación de N partículas en una caja')
    ax.grid(True, alpha=0.3)
    
    # Crear círculos para cada partícula
    circulos = []
    colores = plt.cm.rainbow(np.linspace(0, 1, num_particulas))
    
    for i in range(num_particulas):
        circulo = Circle((x[i, 0], y[i, 0]), radio, 
                        color=colores[i], alpha=0.7)
        ax.add_patch(circulo)
        circulos.append(circulo)
    
    # Texto para mostrar tiempo
    texto_tiempo = ax.text(0.02, 0.98, '', transform=ax.transAxes,
                          verticalalignment='top')
    
    def actualizar(frame):
        """Actualiza la animación"""
        for i, circulo in enumerate(circulos):
            circulo.center = (x[i, frame], y[i, frame])
        texto_tiempo.set_text(f't = {t[frame]:.3f} s')
        return circulos + [texto_tiempo]
    
    # Crear animación
    anim = animation.FuncAnimation(fig, actualizar, frames=num_frames,
                                   interval=20, blit=True, repeat=True)
    
    return fig, anim

def graficar_trayectorias(archivo, num_mostrar=5):
    """Grafica las trayectorias de algunas partículas"""
    datos = cargar_datos(archivo)
    if datos is None:
        return
    
    num_cols = datos.shape[1]
    num_particulas = (num_cols - 1) // 4
    num_mostrar = min(num_mostrar, num_particulas)
    
    t, x, y = extraer_posiciones(datos, num_particulas)
    
    fig, ax = plt.subplots(figsize=(10, 8))
    
    colores = plt.cm.rainbow(np.linspace(0, 1, num_mostrar))
    
    for i in range(num_mostrar):
        ax.plot(x[i], y[i], '-', color=colores[i], 
                alpha=0.6, linewidth=1, label=f'Partícula {i+1}')
        ax.plot(x[i, 0], y[i, 0], 'o', color=colores[i], markersize=8)
        ax.plot(x[i, -1], y[i, -1], 's', color=colores[i], markersize=8)
    
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_title(f'Trayectorias de {num_mostrar} partículas')
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_aspect('equal')
    
    return fig

if __name__ == "__main__":
    # Analizar diferentes experimentos
    experimentos = [
        ("results/gas_diluido.dat", "Gas Diluido"),
        ("results/gas_denso.dat", "Gas Denso"),
        ("results/velocidades.dat", "Distribución de Velocidades")
    ]
    
    for archivo, nombre in experimentos:
        print(f"\n=== {nombre} ===")
        try:
            # Graficar trayectorias
            fig_tray = graficar_trayectorias(archivo, num_mostrar=5)
            if fig_tray:
                plt.savefig(f"results/trayectorias_{nombre.lower().replace(' ', '_')}.png", 
                           dpi=150, bbox_inches='tight')
                print(f"Guardado: trayectorias_{nombre.lower().replace(' ', '_')}.png")
            
            # Crear animación (solo mostrar, no guardar por defecto)
            print(f"Creando animación para {nombre}...")
            fig_anim, anim = animar_particulas(archivo)
            
            # Opcional: guardar como GIF (requiere ImageMagick o ffmpeg)
            # anim.save(f"results/animacion_{nombre.lower().replace(' ', '_')}.gif", 
            #          writer='pillow', fps=30)
            
            plt.close('all')
            
        except Exception as e:
            print(f"Error procesando {archivo}: {e}")
    
    print("\n=== Visualización completada ===")
    print("Las figuras se han guardado en results/")