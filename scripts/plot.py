import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def plot_results(filepath):
    """
    Genera y guarda gráficos a partir de los datos de simulación.
    """
    if not os.path.exists(filepath):
        print(f"Error: El archivo '{filepath}' no fue encontrado.")
        return

    data = pd.read_csv(filepath)
    results_dir = os.path.dirname(filepath)
    plt.style.use('seaborn-v0_8-whitegrid')

    # 1. Theta vs Tiempo
    fig1, ax1 = plt.subplots(figsize=(12, 6))
    ax1.plot(data['t'], data['theta1'], label=r'$\theta_1(t)$', color='blue')
    ax1.plot(data['t'], data['theta2'], label=r'$\theta_2(t)$', color='red', linestyle='--')
    ax1.set_title('Evolución Temporal de los Péndulos', fontsize=16)
    ax1.set_xlabel('Tiempo (s)'); ax1.set_ylabel('Ángulo (rad)')
    ax1.legend(); ax1.grid(True)
    fig1.savefig(os.path.join(results_dir, 'theta_vs_t.png'))
    plt.close(fig1)

    # 2. Espacio de Fase
    fig2, (ax2, ax3) = plt.subplots(1, 2, figsize=(14, 6), sharey=True)
    ax2.plot(data['theta1'], data['omega1'], color='blue', lw=0.5)
    ax2.set_title('Espacio de Fase Péndulo 1'); ax2.set_xlabel(r'$\theta_1$'); ax2.set_ylabel(r'$\omega_1$')
    ax3.plot(data['theta2'], data['omega2'], color='red', lw=0.5)
    ax3.set_title('Espacio de Fase Péndulo 2'); ax3.set_xlabel(r'$\theta_2$')
    fig2.suptitle('Diagramas de Espacio de Fase', fontsize=16)
    fig2.savefig(os.path.join(results_dir, 'phase_space.png'))
    plt.close(fig2)

    # 3. Energías
    fig3, ax4 = plt.subplots(figsize=(12, 6))
    ax4.plot(data['t'], data['E1'], label='Energía Péndulo 1 ($E_1$)', color='blue', alpha=0.7)
    ax4.plot(data['t'], data['E2'], label='Energía Péndulo 2 ($E_2$)', color='red', alpha=0.7)
    ax4.plot(data['t'], data['ET'], label='Energía Total ($E_T$)', color='black', linewidth=2)
    ax4.set_title('Análisis de Energía (Sistema No Conservativo)', fontsize=16)
    ax4.set_xlabel('Tiempo (s)'); ax4.set_ylabel('Energía (J)')
    ax4.legend(); ax4.grid(True)
    fig3.savefig(os.path.join(results_dir, 'energy.png'))
    plt.close(fig3)
    
    print(f"Gráficos generados en '{results_dir}'")

if __name__ == "__main__":
    filepath = sys.argv[1] if len(sys.argv) > 1 else "results/simulation_data.csv"
    plot_results(filepath)