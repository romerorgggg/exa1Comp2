#!/usr/bin/env python3
# Lee todos los archivos results/results_kappa*.csv y genera gráficos comparativos.
# Comentarios: ideal para incluir en el informe; grafica theta1(t) y energía para cada k.
import glob, os, re
import pandas as pd
import matplotlib.pyplot as plt

files = sorted(glob.glob('results/results_kappa*.csv'))
if not files:
    print('No se encontraron archivos results/results_kappa*.csv')
    raise SystemExit(1)

# Extrae kappa del nombre de archivo
def parse_k(fname):
    m = re.search(r'results_kappa([0-9p]+)\.csv', fname)
    if not m: return fname
    return m.group(1).replace('p','.')

os.makedirs('results', exist_ok=True)

# Plot theta1(t) comparativa
plt.figure(figsize=(8,4))
for f in files:
    df = pd.read_csv(f, comment='#', header=None, names=['t','theta1','omega1','theta2','omega2','E'])
    k = parse_k(f)
    plt.plot(df['t'], df['theta1'], label=f'k={k}')
plt.xlabel('t (s)'); plt.ylabel('theta1 (rad)'); plt.legend(); plt.tight_layout()
plt.savefig('results/compare_theta1.png', dpi=200); plt.close()

# Plot theta2(t) comparativa
plt.figure(figsize=(8,4))
for f in files:
    df = pd.read_csv(f, comment='#', header=None, names=['t','theta1','omega1','theta2','omega2','E'])
    k = parse_k(f)
    plt.plot(df['t'], df['theta2'], label=f'k={k}')
plt.xlabel('t (s)'); plt.ylabel('theta2 (rad)'); plt.legend(); plt.tight_layout()
plt.savefig('results/compare_theta2.png', dpi=200); plt.close()

# Plot energia comparativa
plt.figure(figsize=(8,4))
for f in files:
    df = pd.read_csv(f, comment='#', header=None, names=['t','theta1','omega1','theta2','omega2','E'])
    k = parse_k(f)
    plt.plot(df['t'], df['E'], label=f'k={k}')
plt.xlabel('t (s)'); plt.ylabel('E total (J)'); plt.legend(); plt.tight_layout()
plt.savefig('results/compare_energy.png', dpi=200); plt.close()

print('Gráficas comparativas generadas en results/: compare_theta1.png, compare_theta2.png, compare_energy.png')
