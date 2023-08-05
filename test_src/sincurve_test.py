import numpy as np
from math import pi
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

data = np.loadtxt("1_(0A, 0A).txt")	#insert ffile name
y = data

def sin_func(x, A, f, phi, d):
	return A * np.sin(2* np.pi * f * x + phi) + d
	
initial_guess = [1, 60*2*pi, 0, 0]	#declear parmeater

x = np.arange(len(y))
	
popt, pcov = curve_fit(sin_func, x, y, p0=initial_guess)


print("amplitude A = ", popt[0])
print("frequency f = ", popt[1])
print("phase phi = ", popt[2])
print("offset d = ", popt[3])


#plt.plot(x, y, 'b.', label = 'data')
plt.plot(x, sin_func(x, *popt), 'r-', label='Fit')
plt.legend()
plt.show()
