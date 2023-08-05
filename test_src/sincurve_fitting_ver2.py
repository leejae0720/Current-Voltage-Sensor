import serial
import time
import datetime
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

ser = serial.Serial('/dev/ttyUSB0', 115200)

directory = '/home/irms/'

data = []

while True:
	line = ser.readline().decode().strip()
	
	data.append(line)
	print(line)
	if len(data) == 10:
		current_time = datetime.datetime.now()
		
		file_name = current_time.strftime("%Y%m%d%H%M%S") + ".txt"
		
		with open(directory + file_name, 'w') as file:
			for item in data:
				file.write("%s\n" % item)
				
		x = np.arange(len(data))
		y = np.array(data, dtype=float)
		
		def sin_func(x, a, b, c, d):
			return a*np.sin(b * x + c) + d
		
		popt, pcov =curve_fit(sin_func, x, y)
		
		plt.plot(x, y, 'b-', label='data')
		plt.plot(x, sin_func(x, *popt), 'r', label='Fitted Curve')
		plt.legend()
		plt.xlabel('Time')
		plt.ylabel('Data')
		plt.title('Sin Curve Fitting')
		plt.show()
		
		break
