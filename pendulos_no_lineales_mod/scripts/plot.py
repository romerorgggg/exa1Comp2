#!/usr/bin/env python3
import sys, os
import numpy as np, pandas as pd, matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("Uso: python3 scripts/plot.py results/results.csv")
    sys.exit(1)

fname = sys.argv[1]
df = pd.read_csv(fname, comment='#', header=None,
                 names=['t','theta1','omega1','theta2','omega2','E'])

os.makedirs("results", exist_ok=True)

plt.figure(figsize=(8,4))
plt.plot(df['t'], df['theta1'], label='theta1')
plt.plot(df['t'], df['theta2'], label='theta2', alpha=0.8)
plt.xlabel('t (s)'); plt.ylabel('theta (rad)'); plt.legend(); plt.tight_layout()
plt.savefig('results/theta_vs_t.png', dpi=200); plt.close()

plt.figure(figsize=(6,6))
plt.plot(df['theta1'], df['omega1'], label='pendulo 1', lw=0.6)
plt.plot(df['theta2'], df['omega2'], label='pendulo 2', lw=0.6)
plt.xlabel('theta (rad)'); plt.ylabel('omega (rad/s)'); plt.legend(); plt.tight_layout()
plt.savefig('results/phase_space.png', dpi=200); plt.close()

plt.figure(figsize=(8,4))
plt.plot(df['t'], df['E'])
plt.xlabel('t (s)'); plt.ylabel('E total (J)'); plt.tight_layout()
plt.savefig('results/energy.png', dpi=200); plt.close()
print("Figuras guardadas en results/: theta_vs_t.png, phase_space.png, energy.png")
