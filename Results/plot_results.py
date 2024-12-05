import matplotlib.pyplot as plt
import numpy as np

freq_spectrum = []
freq_hz = []

i = 0

Fs = 30 # Sample frequency in Hz
N = 256 # Number of sample points

with open('FilteredPPG.txt') as f:
    for line in f:
        if i <= N/2:
            freq_spectrum.append( float(line) )
            freq_hz.append((i*(Fs/N)))
            i = i + 1
        else:
            break



plt.plot(np.array(freq_hz), np.array(freq_spectrum))
plt.title("Frequency spectrum of PPG signal")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude")
plt.show()

