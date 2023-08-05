import numpy as np
from math import pi
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

data = np.loadtxt("20230508171235.txt")	#insert file name
y1 = data[:, 0]
y2 = data[:, 1]

def sin_func(x, A, f, phi, d):
	return A * np.sin(2* np.pi * f * x + phi) + d
	
initial_guess = [0.0000000000001, 60*2*pi, 0, 0]	#declear parmeater

params1, _ = curve_fit(sin_func, np.arange(len(y1)), y1, p0 = initial_guess)
params2, _ = curve_fit(sin_func, np.arange(len(y2)), y2, p0 = initial_guess)

x = np.linspace(0, len(y1)-1, len(y1)*10)

plt.plot(x, sin_func(x, *params1), label= 'y1 fit')
plt.plot(x, sin_func(x, *params2), label= 'y2 fit')
plt.legend()
plt.show()
