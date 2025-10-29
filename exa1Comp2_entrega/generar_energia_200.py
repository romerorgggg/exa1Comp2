#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

def cargar_datos(archivo):
    try:
        datos = np.loadtxt(archivo)
        return datos
    except Exception as e:
        print(f"Error: {e}")
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

print("Generando grafica de energia para 200 particulas...")
datos = cargar_datos("results/velocidades.dat")

if datos is None:
    print("Error: No se pudo cargar el archivo")
    exit(1)

t, energia = calcular_energia_total(datos)
E0 = energia[0]
E_final = energia[-1]
error_relativo = abs(E_final - E0) / E0 * 100

print(f"Energia inicial: {E0:.6f}")
print(f"Energia final: {E_final:.6f}")
print(f"Error relativo: {error_relativo:.4f}%")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))

ax1.plot(t, energia, 'b-', linewidth=1, alpha=0.7)
ax1.axhline(y=E0, color='r', linestyle='--', linewidth=2, label=f'E0 = {E0:.3f}')
ax1.set_xlabel('Tiempo (s)')
ax1.set_ylabel('Energia Cinetica Total')
ax1.set_title(f'200 Particulas - Error relativo: {error_relativo:.4f}%')
ax1.legend()
ax1.grid(True, alpha=0.3)

error_rel = (energia - E0) / E0 * 100
ax2.plot(t, error_rel, 'r-', linewidth=1)
ax2.axhline(y=0, color='k', linestyle='--', linewidth=1)
ax2.set_xlabel('Tiempo (s)')
ax2.set_ylabel('Error relativo (%)')
ax2.set_title('Error en la conservacion de energia')
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("results/energia_200_particulas.png", dpi=150, bbox_inches='tight')
print("\nGuardado: results/energia_200_particulas.png")
plt.close()
