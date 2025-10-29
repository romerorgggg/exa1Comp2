#!/usr/bin/env python3
"""
Genera diagrama de flujo del programa
Física Computacional 2 - 2025
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch

def crear_diagrama():
    """Crea el diagrama de flujo del programa principal"""
    
    fig, ax = plt.subplots(figsize=(10, 14))
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 20)
    ax.axis('off')
    
    # Función auxiliar para crear cajas
    def caja(x, y, ancho, alto, texto, color='lightblue'):
        box = FancyBboxPatch((x, y), ancho, alto,
                             boxstyle="round,pad=0.1",
                             edgecolor='black', facecolor=color,
                             linewidth=2)
        ax.add_patch(box)
        ax.text(x + ancho/2, y + alto/2, texto,
                ha='center', va='center', fontsize=9, weight='bold')
    
    # Función para crear flechas
    def flecha(x1, y1, x2, y2):
        arrow = FancyArrowPatch((x1, y1), (x2, y2),
                               arrowstyle='->', mutation_scale=20,
                               linewidth=2, color='black')
        ax.add_patch(arrow)
    
    # Función para diamantes (decisión)
    def diamante(x, y, ancho, alto, texto):
        points = [(x + ancho/2, y + alto),
                 (x + ancho, y + alto/2),
                 (x + ancho/2, y),
                 (x, y + alto/2)]
        polygon = patches.Polygon(points, closed=True,
                                 edgecolor='black', facecolor='lightyellow',
                                 linewidth=2)
        ax.add_patch(polygon)
        ax.text(x + ancho/2, y + alto/2, texto,
                ha='center', va='center', fontsize=8, weight='bold')
    
    # Título
    ax.text(5, 19, 'Diagrama de Flujo - Simulación N Partículas',
            ha='center', fontsize=14, weight='bold')
    
    # Flujo principal
    y = 18
    caja(3, y, 4, 0.8, 'INICIO', 'lightgreen')
    
    y -= 1.2
    flecha(5, y + 1.2, 5, y + 0.8)
    caja(2.5, y, 5, 0.8, 'Inicializar parámetros\n(N, W, H, dt, t_final)')
    
    y -= 1.2
    flecha(5, y + 1.2, 5, y + 0.8)
    caja(2, y, 6, 0.8, 'Crear objeto Caja(W, H)')
    
    y -= 1.2
    flecha(5, y + 1.2, 5, y + 0.8)
    caja(2, y, 6, 1, 'Inicializar N partículas\n(rejilla o aleatorio)')
    
    y -= 1.5
    flecha(5, y + 1.5, 5, y + 1)
    caja(2.5, y, 5, 1, 't = 0\nE₀ = EnergíaTotal()')
    
    # Loop principal
    y -= 1.5
    flecha(5, y + 1.5, 5, y + 1)
    diamante(3.5, y, 3, 1, 't < t_final?')
    
    # Rama NO (salir)
    flecha(6.5, y + 0.5, 8, y + 0.5)
    ax.text(7.5, y + 0.7, 'NO', ha='center', fontsize=9)
    caja(8, y, 1.5, 1, 'FIN', 'lightcoral')
    
    # Rama SÍ (continuar)
    y -= 1.5
    flecha(5, y + 1.5, 5, y + 1.2)
    ax.text(5.3, y + 1.3, 'SÍ', ha='center', fontsize=9)
    
    # Bloque de simulación
    caja(1.5, y, 7, 0.8, 'Mover partículas (Euler o Verlet)')
    
    y -= 1
    flecha(5, y + 1, 5, y + 0.8)
    caja(1.5, y, 7, 0.8, 'Detectar y resolver choques con paredes')
    
    y -= 1
    flecha(5, y + 1, 5, y + 0.8)
    caja(1.5, y, 7, 0.8, 'Detectar colisiones entre partículas\n(O(N²) - todos los pares)')
    
    y -= 1
    flecha(5, y + 1, 5, y + 0.8)
    caja(1.5, y, 7, 0.8, 'Resolver colisiones elásticas')
    
    y -= 1.2
    flecha(5, y + 1.2, 5, y + 0.8)
    diamante(3.5, y, 3, 0.8, 't % dt_out == 0?')
    
    # Rama SÍ (guardar)
    flecha(6.5, y + 0.4, 8, y + 0.4)
    ax.text(7.5, y + 0.6, 'SÍ', ha='center', fontsize=8)
    caja(8, y - 0.2, 1.8, 0.8, 'Guardar estado')
    
    # Rama NO (continuar)
    ax.text(2.8, y + 0.4, 'NO', ha='center', fontsize=8)
    
    y -= 1.2
    flecha(5, y + 1.2, 5, y + 0.8)
    caja(2.5, y, 5, 0.8, 't = t + dt')
    
    # Volver al inicio del loop
    flecha(5, y, 1, y)
    flecha(1, y, 1, y + 8)
    flecha(1, y + 8, 3.5, y + 8)
    
    plt.tight_layout()
    return fig

def crear_diagrama_clases():
    """Crea diagrama de clases UML simplificado"""
    
    fig, ax = plt.subplots(figsize=(12, 8))
    ax.set_xlim(0, 12)
    ax.set_ylim(0, 10)
    ax.axis('off')
    
    # Título
    ax.text(6, 9.5, 'Diagrama de Clases UML', 
            ha='center', fontsize=14, weight='bold')
    
    # Clase Bola
    bola_x, bola_y = 1, 4
    rect1 = patches.Rectangle((bola_x, bola_y), 3.5, 4,
                              linewidth=2, edgecolor='black', 
                              facecolor='lightblue')
    ax.add_patch(rect1)
    
    # Nombre de clase
    ax.plot([bola_x, bola_x + 3.5], [bola_y + 3.5, bola_y + 3.5], 'k-', linewidth=2)
    ax.text(bola_x + 1.75, bola_y + 3.7, 'Bola', 
            ha='center', fontsize=12, weight='bold')
    
    # Atributos
    ax.plot([bola_x, bola_x + 3.5], [bola_y + 2.5, bola_y + 2.5], 'k-', linewidth=2)
    atributos = ['- x_, y_: double',
                 '- vx_, vy_: double',
                 '- masa_: double',
                 '- radio_: double']
    for i, attr in enumerate(atributos):
        ax.text(bola_x + 0.1, bola_y + 2.3 - i*0.25, attr, 
                fontsize=8, family='monospace')
    
    # Métodos
    metodos = ['+ MoverseEuler(dt)',
               '+ MoverseVerlet(dt)',
               '+ RebotePared(W, H)',
               '+ Choque(Bola&)',
               '+ EnergíaCinética()']
    for i, met in enumerate(metodos):
        ax.text(bola_x + 0.1, bola_y + 1.0 - i*0.2, met, 
                fontsize=8, family='monospace')
    
    # Clase Caja
    caja_x, caja_y = 7, 4
    rect2 = patches.Rectangle((caja_x, caja_y), 4, 4,
                              linewidth=2, edgecolor='black', 
                              facecolor='lightgreen')
    ax.add_patch(rect2)
    
    ax.plot([caja_x, caja_x + 4], [caja_y + 3.5, caja_y + 3.5], 'k-', linewidth=2)
    ax.text(caja_x + 2, caja_y + 3.7, 'Caja', 
            ha='center', fontsize=12, weight='bold')
    
    ax.plot([caja_x, caja_x + 4], [caja_y + 2.5, caja_y + 2.5], 'k-', linewidth=2)
    atributos_caja = ['- ancho_: double',
                      '- alto_: double',
                      '- bolas_: vector<Bola>',
                      '- num_choques_: int']
    for i, attr in enumerate(atributos_caja):
        ax.text(caja_x + 0.1, caja_y + 2.3 - i*0.25, attr, 
                fontsize=8, family='monospace')
    
    metodos_caja = ['+ InicializarRejilla()',
                    '+ EvolucionarVerlet(dt)',
                    '+ DetectarColisiones()',
                    '+ EnergíaTotal()',
                    '+ SimularCompleto()']
    for i, met in enumerate(metodos_caja):
        ax.text(caja_x + 0.1, caja_y + 1.0 - i*0.2, met, 
                fontsize=8, family='monospace')
    
    # Relación: Caja contiene Bolas
    arrow = FancyArrowPatch((caja_x, caja_y + 2), (bola_x + 3.5, caja_y + 2),
                           arrowstyle='<-', mutation_scale=20,
                           linewidth=2, color='black')
    ax.add_patch(arrow)
    ax.text(5.5, caja_y + 2.3, '1..N', ha='center', fontsize=10, weight='bold')
    ax.text(5.5, caja_y + 1.7, 'contiene >', ha='center', fontsize=9)
    
    plt.tight_layout()
    return fig

if __name__ == "__main__":
    print("Generando diagramas...")
    
    # Diagrama de flujo
    fig1 = crear_diagrama()
    fig1.savefig('documents/diagrama_flujo.png', dpi=300, bbox_inches='tight')
    print("✓ Guardado: documents/diagrama_flujo.png")
    
    # Diagrama de clases
    fig2 = crear_diagrama_clases()
    fig2.savefig('documents/diagrama_clases.png', dpi=300, bbox_inches='tight')
    print("✓ Guardado: documents/diagrama_clases.png")
    
    print("\n¡Diagramas generados exitosamente!")
    plt.close('all')