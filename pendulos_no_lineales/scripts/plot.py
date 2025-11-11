#!/usr/bin/env python3
import os, glob
import numpy as np, pandas as pd, matplotlib.pyplot as plt
from scipy.fft import rfft, rfftfreq

outdir = "results"
os.makedirs(outdir, exist_ok=True)

def plot_energy(csvfile):
    df = pd.read_csv(csvfile)
    t = df['t']
    th1 = df['theta1']; th2 = df['theta2']
    w1 = df['omega1']; w2 = df['omega2']
    E1 = df['E1']; E2 = df['E2']; Et = df['E_total']

    # 1) theta1 & theta2 vs t (overlay)
    plt.figure(figsize=(9,4))
    plt.plot(t, th1, label='θ1(t)')
    plt.plot(t, th2, label='θ2(t)', alpha=0.8)
    plt.xlabel('t (s)'); plt.ylabel('θ (rad)'); plt.legend(); plt.tight_layout()
    plt.savefig(f"{outdir}/theta_vs_t.png"); plt.close()

    # 2) omega1 & omega2 vs t
    plt.figure(figsize=(9,4))
    plt.plot(t, w1, label='ω1(t)')
    plt.plot(t, w2, label='ω2(t)', alpha=0.8)
    plt.xlabel('t (s)'); plt.ylabel('ω (rad/s)'); plt.legend(); plt.tight_layout()
    plt.savefig(f"{outdir}/omega_vs_t.png"); plt.close()

    # 3) E1, E2, E_total vs t
    plt.figure(figsize=(9,4))
    plt.plot(t, E1, label='E1')
    plt.plot(t, E2, label='E2')
    plt.plot(t, Et, label='E_total', alpha=0.7)
    plt.xlabel('t (s)'); plt.ylabel('E (J)'); plt.legend(); plt.tight_layout()
    plt.savefig(f"{outdir}/energies_vs_t.png"); plt.close()

    # 4) E1 vs E2 scatter (transferencia de energía)
    plt.figure(figsize=(6,6))
    plt.plot(E1, E2, '.', ms=2)
    plt.xlabel('E1'); plt.ylabel('E2'); plt.tight_layout()
    plt.savefig(f"{outdir}/E1_vs_E2.png"); plt.close()

    # 5) Phase space θ1 vs ω1
    plt.figure(figsize=(6,6))
    plt.plot(th1, w1, '.', ms=0.8)
    plt.xlabel('θ1'); plt.ylabel('ω1'); plt.tight_layout()
    plt.savefig(f"{outdir}/phase_theta1_omega1.png"); plt.close()

    # 6) Phase space θ2 vs ω2
    plt.figure(figsize=(6,6))
    plt.plot(th2, w2, '.', ms=0.8)
    plt.xlabel('θ2'); plt.ylabel('ω2'); plt.tight_layout()
    plt.savefig(f"{outdir}/phase_theta2_omega2.png"); plt.close()

    # 7) Poincaré section (sample at forcing phase)
    if 'E1' in df.columns and 'theta1' in df.columns:
        try:
            omega_f = df.get('omega_f', pd.Series([1.0]*len(df))).iloc[0]
        except:
            omega_f = 1.0
    else:
        omega_f = 1.0
    phases = (t * omega_f) % (2*np.pi)
    mask = np.abs(phases) < (2*np.pi*0.005)  # sección fina
    plt.figure(figsize=(6,6))
    plt.plot(df['theta1'][mask], df['omega1'][mask], '.', ms=1)
    plt.xlabel('θ1 (Poincaré)'); plt.ylabel('ω1'); plt.tight_layout()
    plt.savefig(f"{outdir}/poincare_theta1.png"); plt.close()

    # 8) FFT de theta1 (espectro)
    dt = t.iloc[1] - t.iloc[0] if len(t) > 1 else 0.001
    yf = rfft(th1 - th1.mean())
    xf = rfftfreq(len(th1), dt)
    plt.figure(figsize=(8,4))
    plt.plot(xf, np.abs(yf))
    plt.xlim(0, 5)
    plt.xlabel('freq (Hz)'); plt.ylabel('FFT amp'); plt.tight_layout()
    plt.savefig(f"{outdir}/fft_theta1.png"); plt.close()

    print("Saved energy-related plots.")

def plot_resonance(csvfile):
    df = pd.read_csv(csvfile)
    omega = df['omega_f']; amp = df['max_amp_theta1']
    plt.figure(figsize=(8,4))
    plt.plot(omega, amp, '-o')
    plt.xlabel('omega_f'); plt.ylabel('max |theta1|'); plt.tight_layout()
    plt.savefig(f"{outdir}/resonance_curve.png"); plt.close()
    plt.figure(figsize=(6,4))
    plt.hist(amp, bins=20); plt.xlabel('max amp'); plt.ylabel('count'); plt.tight_layout()
    plt.savefig(f"{outdir}/resonance_hist.png"); plt.close()
    print("Saved resonance plots.")

def plot_chaos(csvfile):
    df = pd.read_csv(csvfile)
    # Reuse energy plots for chaos run
    plot_energy(csvfile)
    print("Saved chaos-related plots (reused energy plots + Poincaré + FFT).")

def main():
    # Energy file
    if os.path.exists(f"{outdir}/energy_transfer.csv"):
        plot_energy(f"{outdir}/energy_transfer.csv")
    # Resonance sweep
    if os.path.exists(f"{outdir}/resonance_sweep.csv"):
        plot_resonance(f"{outdir}/resonance_sweep.csv")
    # Chaos run
    if os.path.exists(f"{outdir}/chaos_run.csv"):
        plot_chaos(f"{outdir}/chaos_run.csv")
    print("Plotting finished. Check the results/ folder for PNG files.")

if __name__ == "__main__":
    main()
